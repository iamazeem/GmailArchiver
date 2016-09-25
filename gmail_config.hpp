#ifndef GMAIL_CONFIG_HPP_
#define GMAIL_CONFIG_HPP_

#include <string>

/*** GMail Archiver Configurations ***/

namespace GmailArchiver {

using std::string;

namespace Config {
static const string SERVICE_NAME { "Gmail" };
static const string CLIENT_KEY   { "569712658766-ck18e8rgpb4pskvpsbpvh9uv27orqqs0.apps.googleusercontent.com" };
static const string SECRET       { "4mz8JDqSAjCIr6TkX5DWqPzi" };
static const string AUTH_URI     { "https://accounts.google.com/o/oauth2/auth"  };
static const string TOKEN_URI    { "https://accounts.google.com/o/oauth2/token" };
static const string REDIRECT_URI { "http://localhost:8888" };
} /* Configuration */

namespace Scopes {
static const string FULL         { "https://mail.google.com" };
static const string READ_ONLY    { "https://www.googleapis.com/auth/gmail.readonly" };
static const string COMPOSE      { "https://www.googleapis.com/auth/gmail.compose"  };
static const string MODIFY       { "https://www.googleapis.com/auth/gmail.modify"   };
} /* Scopes */

namespace RestAPIs {
static const string MESSAGES     { "https://www.googleapis.com/gmail/v1/users/me/messages" };
} /* RestAPIs */

} /* GmailTest */

#endif /* GMAIL_CONFIG_HPP_ */
