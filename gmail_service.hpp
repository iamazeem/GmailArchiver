#ifndef GMAIL_SERVICE_HPP_
#define GMAIL_SERVICE_HPP_

#include <cpprest/http_client.h>

namespace GmailArchiver {

using namespace web::http::client;
using namespace web::http::oauth2::experimental;

using std::string;

class GmailService
{
public:
    GmailService( const string& scope,
                  const string& clientId,
                  const string& clientSecret,
                  const string& authUri,
                  const string& tokenUri,
                  const string& redirectUri );

    void start( const string filename,
                const string startDate,
                const string endDate ) noexcept;

private:
    bool validateArgs( const string filename,
                       const string startDate,
                       const string endDate ) noexcept;

    bool isAuthorized( void ) noexcept;

    http_client_config      _clientConfig;
    oauth2_config           _oauthConfig;
};

} /* GmailTest */

#endif /* GMAIL_SERVICE_HPP_ */
