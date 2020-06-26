#ifndef GMAIL_UTILS_HPP_
#define GMAIL_UTILS_HPP_

#include <string>

// Utilities for GmailArchiver

namespace GmailArchiver {
namespace Utils {

namespace Crypto {

    void encrypt ( std::string& str ) noexcept;
    void decrypt ( std::string& str ) noexcept;

} // Crypto

// Utils functions
bool openBrowser( const std::string& uri ) noexcept;
bool isValidDateFormat( const std::string& date ) noexcept;

} // Utils
} // GmailArchiver

#endif // GMAIL_UTILS_HPP_
