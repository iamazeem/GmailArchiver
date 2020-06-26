#include "gmail_oauth2.hpp"
#include "logger.hpp"

namespace GmailArchiver {

OAuth2::OAuth2( oauth2_config& oauthConfig )
    :
    _authCodeListener{ oauthConfig.redirect_uri() },
    _oauthConfig{ oauthConfig }
{
    _authCodeListener.support( methods::GET,
                               std::bind( &OAuth2::authCodeHandler, this, std::placeholders::_1 ) );

    _authCodeListener.open().wait();
}

OAuth2::~OAuth2()
{
    _authCodeListener.close().wait();
}

bool OAuth2::isAuthComplete( void ) noexcept
{
    return create_task( _tceFlag ).get();
}

void OAuth2::authCodeHandler( http_request request ) noexcept
{
    if ( request.request_uri().path() == "/" &&
         !request.request_uri().query().empty() )
    {
        try
        {
            _oauthConfig.token_from_redirected_uri( request.request_uri() ).get();
            _tceFlag.set( true );

            request.reply( status_codes::OK, "Authorization Successful! :)" ).get();
        }
        catch ( const oauth2_exception& e )
        {
            _tceFlag.set( false );
            LOG_ERR() << "Error: " << e.what() << std::endl;
            request.reply( status_codes::NotFound, "OAuth2 Exception!" ).get();
        }
    }
    else
    {
        _tceFlag.set( false );
        LOG_ERR() << "Error: Invalid URI path and/or query!\n";
        request.reply( status_codes::NotFound, "Error: Invalid URI path and/or query!" ).get();
    }
}

} // GmailArchiver

