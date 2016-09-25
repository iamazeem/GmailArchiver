#ifndef GMAIL_UTILS_HPP_
#define GMAIL_UTILS_HPP_

#include <string>

/*** Utilities for GmailTest ***/

namespace GmailArchiver {
namespace Utils {

namespace Crypto {

    void encrypt ( std::string& str ) noexcept;
    void decrypt ( std::string& str ) noexcept;

} /* Crypto */

/* Utils functions */
bool openBrowser( const std::string& uri ) noexcept;
bool isValidDateFormat( const std::string& date ) noexcept;

} /* Utils */
} /* GmailTest */

#endif /* GMAIL_UTILS_HPP_ */
