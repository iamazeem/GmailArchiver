#include <cstdlib>
#include <regex>
#include "gmail_utils.hpp"
#include "logger.hpp"

namespace GmailArchiver {
namespace Utils {

// Minimal XOR-based Crypto-lib
namespace Crypto {

    static const char XOR_CRYPTO_CHAR = '@';

    void encrypt ( std::string& str ) noexcept
    {
        for ( auto& s : str ) { s ^= XOR_CRYPTO_CHAR; }
    }

    void decrypt ( std::string& str ) noexcept
    {
        for ( auto& s : str ) { s ^= XOR_CRYPTO_CHAR; }
    }

} // Crypto

// Utils functions
bool openBrowser( const std::string& uri ) noexcept
{
#if defined(__APPLE__)
    const std::string command { R"(open ")" + uri + R"(")" };
#else
    const std::string command { R"(xdg-open ")" + uri + R"(")" };
#endif

    LOG_INF() << "Opening browser and waiting for authentication...\n";

    if ( system( command.c_str() ) < 0 )
    {
        LOG_ERR() << "Error while opening browser!\n";
        return false;
    }

    return true;
}

bool isValidDateFormat( const std::string& date ) noexcept
{
    const std::regex regexDate { R"(\d{4}/\d{2}/\d{2})" };
    std::smatch match;
    std::regex_match( date, match, regexDate );
    return !match.str().empty();
}

} // Utils
} // GmailArchiver
