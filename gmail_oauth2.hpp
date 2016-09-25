#ifndef GMAIL_OAUTH2_HPP_
#define GMAIL_OAUTH2_HPP_

#include <cpprest/oauth2.h>
#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>

namespace GmailArchiver {

using namespace pplx;
using namespace web::http;
using namespace web::http::oauth2::experimental;
using namespace web::http::experimental::listener;

class OAuth2
{
public:
    OAuth2( oauth2_config& oauthConfig );

    ~OAuth2();

    bool isAuthComplete( void ) noexcept;

private:
    void authCodeHandler( http_request request ) noexcept;

    http_listener                   _authCodeListener;
    oauth2_config&                  _oauthConfig;
    task_completion_event<bool>     _tceFlag;
};

} /* GmailTest */

#endif /* GMAIL_OAUTH2_HPP_ */
