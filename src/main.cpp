/*
 *  GMail Archiver to archive email messages in encrypted format.
 *
 *  Written by: Azeem Sajid
 *  Email:      azeem.sajid@gmail.com
 *  GitHub:     https://github.com/iamAzeem
 *  LinkedIn:   https://pk.linkedin.com/in/az33msajid
 *
 *  Syntax:     ./GmailArchiver <filename> <start-date> <end-date>
 *
 *  Arguments:
 *  1. <filename>   - output filename to save encrypted email messages
 *  2. <start-date> - starting date of email messages to be archived
 *  3. <end-date>   - ending date of email messages to be archived
 *
 *  NOTE: Starting and Ending dates should be in the format [ YYYY/MM/DD ].
 *
 * */

#include <cstdlib>

#include "gmail_config.hpp"
#include "gmail_service.hpp"

int main( int argc, char *argv[] )
{
    constexpr unsigned int REQUIRED_NO_OF_ARGS = 4;

    if ( argc != REQUIRED_NO_OF_ARGS )
    {
        std::cout << "SYNTAX:  ./GmailArchiver <filename> <start-date> <end-date>\n"
                  << "EXAMPLE: ./GmailArchiver  mybackup   2016/01/01  2016/01/15\n"
                  << "NOTE:    Date must be in the format [YYYY/MM/DD] e.g. 2016/01/30\n";

        return EXIT_FAILURE;
    }

    using namespace GmailArchiver;

    GmailService gmail { Scopes::READ_ONLY,
                         Config::CLIENT_KEY,
                         Config::SECRET,
                         Config::AUTH_URI,
                         Config::TOKEN_URI,
                         Config::REDIRECT_URI };

    gmail.start( argv[1], argv[2], argv[3] );

    return EXIT_SUCCESS;
}
