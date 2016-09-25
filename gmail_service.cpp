#include <fstream>
#include <queue>

#include "gmail_service.hpp"
#include "gmail_oauth2.hpp"
#include "gmail_config.hpp"
#include "gmail_utils.hpp"
#include "logger.hpp"

namespace GmailArchiver {

using namespace web;
using namespace utility;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::oauth2::experimental;
using namespace web::http::experimental::listener;

/* Local Utility Function Declarations */
static
bool fetchMsgIdsList( http_client&        client,
                      std::queue<string>& queue,
                      const string&       startDate,
                      const string&       endDate ) noexcept;

static
bool fetchAndArchive( http_client&        client,
                      std::queue<string>& queue,
                      const string&       filename ) noexcept;

/* GmailService class definitions */
GmailService::GmailService( const string& scope,
                            const string& clientId,
                            const string& clientSecret,
                            const string& authUri,
                            const string& tokenUri,
                            const string& redirectUri )
                            :
                            _oauthConfig{ clientId, clientSecret, authUri, tokenUri, redirectUri }
{
    _oauthConfig.set_scope( scope );
}

void GmailService::start( const string filename,
                          const string startDate,
                          const string endDate ) noexcept
{
    LOG_INF() << "GMail Archiver started!\n";

    /* Validate output file, start and end dates */
    if ( !validateArgs( filename, startDate, endDate ) )
    {
        return;
    }

    LOG_INF() << "OAuth 2.0: Waiting for authentication and authorization...\n";

    /* Authenticate and authorize current user */
    if ( !isAuthorized() )
    {
        LOG_ERR() << "Authorization failed!\n";
        return;
    }

    LOG_INF() << "Authorization completed!\n";

    try
    {
        /* Set up OAuth 2.0 configuration for HTTP client */
        _clientConfig.set_oauth2( _oauthConfig );

        /* Set up HTTP client */
        http_client client { RestAPIs::MESSAGES, _clientConfig };

        /* Message IDs queue */
        std::queue<string> msgIdsQueue;

        /* Fetch message IDs in queue; fetch email message and archive */
        if ( fetchMsgIdsList( client, msgIdsQueue, startDate, endDate ) )
        {
            fetchAndArchive( client, msgIdsQueue, filename );
        }
    }
    catch ( const std::exception& e )
    {
        LOG_ERR() << "Error: " << e.what() << std::endl;
    }

    LOG_INF() << "Bye-bye! :)\n";
}

bool GmailService::validateArgs( const string filename,
                                 const string startDate,
                                 const string endDate ) noexcept
{
    LOG_INF() << "Validating arguments...\n";

    /* Validate filename for existing files */
    std::ifstream infile { filename };
    if( infile.good() )
    {
        LOG_ERR() << filename << " already exists! Try other names!\n";
        infile.close();
        return false;
    }

    /* Validate start date format */
    if ( !Utils::isValidDateFormat( startDate ) )
    {
        LOG_ERR() << "Invalid <start-date> format i.e. " << startDate << std::endl;
        return false;
    }

    /* Validate end date format */
    if ( !Utils::isValidDateFormat( endDate ) )
    {
        LOG_ERR() << "Invalid <end-date> format i.e. " << endDate << std::endl;
        return false;
    }

    LOG_INF() << "Arguments validation completed!\n";
    return true;
}

bool GmailService::isAuthorized( void ) noexcept
{
    OAuth2 oauth2 { _oauthConfig };
    const string authUri { _oauthConfig.build_authorization_uri( true ) };
    return Utils::openBrowser( authUri ) && oauth2.isAuthComplete();
}

/* Local Utility Function Definitions */
static
bool fetchMsgIdsList( http_client&        client,
                      std::queue<string>& queue,
                      const string&       startDate,
                      const string&       endDate ) noexcept
{
    /* Build URI with query for fetching messages IDs list */
    const string msgsQueryWithDate { R"(q="after:)" + startDate + R"( before:)" + endDate + R"(")" };

    uri_builder msgsListUriBuilder;
    msgsListUriBuilder.set_query( R"(maxResults=500)", true );
    msgsListUriBuilder.append_query( msgsQueryWithDate, true );
    msgsListUriBuilder.append_query( R"(fields=messages,nextPageToken)", true );

    LOG_INF() << "Fetching and populating email message IDs list...\n";

    /* Send request to fetch email messages IDs list */
    auto rspBody = client.request( methods::GET, msgsListUriBuilder.to_string() ).get().content_ready().get();

    /* Create queue from the IDs; fetch all the IDs from next pages also */
    bool isComplete { true };
    bool isNextPageAvailable { true };

    while ( isNextPageAvailable )
    {
        /* Extract response JSON */
        const auto jsonRsp = rspBody.extract_json().get();

        /* Check if "messages" object exists in the JSON response */
        if ( !jsonRsp.has_field( "messages" ) )
        {
            LOG_ERR() << "No message found within specified time period!\n";
            isComplete = false;
            break;
        }

        /* Fetch if there are messages in the response */
        const auto msgs = jsonRsp.at( "messages" );
        LOG_INF() << "Fetched " << msgs.as_array().size() << " message IDs!\n";

        for ( auto msg : msgs.as_array() )
        {
            auto msgId = msg.at( "id" ).as_string();
            queue.push( msgId );
        }

        /* Fetch next page if there is any */
        isNextPageAvailable = jsonRsp.has_field( "nextPageToken" );
        if ( isNextPageAvailable )
        {
            auto nextPageToken = jsonRsp.at( "nextPageToken" ).as_string();

            /* Build URI for next page */
            uri_builder nxtPageUriBuilder { msgsListUriBuilder };
            nxtPageUriBuilder.append_query( "pageToken=" + nextPageToken, true );

            /* Send request to fetch next page */
            rspBody = client.request( methods::GET, nxtPageUriBuilder.to_string() ).get().content_ready().get();
        }
    }

    if ( isComplete )
    {
        LOG_INF() << "Total " << queue.size() << " message IDs fetched successfully!\n";
    }

    return isComplete;
}

static
bool fetchAndArchive( http_client&        client,
                      std::queue<string>& queue,
                      const string&       filename ) noexcept
{
    /* Open new file to archive messages */
    std::ofstream archiveFile { filename };

    if ( !archiveFile.is_open() )
    {
        LOG_ERR() << "Error while creating archive file! Aborting...\n";
        return false;
    }

    LOG_INF() << "Starting to archive email messages... Please wait...\n";

    /* Store opening bracket to start correct JSON array */
    string startingBracket { "[" };
    Utils::Crypto::encrypt( startingBracket );
    archiveFile.write( startingBracket.c_str(), 1 );

    /* Store closing bracket to complete correct JSON object */
    string endingBracket { "]" };
    Utils::Crypto::encrypt( endingBracket );

    /* Encrypt comma separator to append messages in JSON array */
    string separatingComma { "," };
    Utils::Crypto::encrypt( separatingComma );

    try
    {
        /* Iterate through IDs list and fetch email message */
        for ( size_t i = 0; i < queue.size(); ++i )
        {
            /* Build URI for current message ID */
            uri_builder msgUriBuilder { "/" + queue.front() };
            msgUriBuilder.set_query( R"(fields=id,payload)", true );

            /* Remove current message ID from list */
            queue.pop();

            /* Send request to fetch current message with ID and payload */
            auto msgRsp = client.request( methods::GET, msgUriBuilder.to_string() ).get().content_ready().get();
            auto msgRspAsString = msgRsp.extract_string().get();

            /* Write current message to archive file */
            Utils::Crypto::encrypt( msgRspAsString );
            archiveFile.write( msgRspAsString.c_str(), msgRspAsString.size() );

            /* Append a comma after each message; but not with the last one */
            if ( (queue.size() > 1) && (i != queue.size() - 1) )
            {
                archiveFile.write( separatingComma.c_str(), 1 );
            }
        }
    }
    catch ( const std::exception& e )
    {
        archiveFile.close();

        LOG_ERR() << e.what() << std::endl;
        LOG_WRN() << "Do NOT use archived file! It might be corrupted!\n";
        return false;
    }

    /* Write ending bracket to complete the JSON array */
    archiveFile.write( endingBracket.c_str(), 1 );

    /* Close archive file */
    archiveFile.close();

    LOG_INF() << "Archive [" << filename << "] created successfully!\n";

    return true;
}

} /* GmailTest */
