#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <iostream>

/*** Minimal Logger ***/

/* Logging Macros */
#define LOG_FMT(LVL) \
        LVL << " : "                    \
            << __FILE__     << " : "    \
            << __FUNCTION__ << "(): "   \
            << __LINE__     << ": "

#define LOG_INF() \
        std::clog << LOG_FMT( "[INF]" )

#define LOG_WRN() \
        std::clog << LOG_FMT( "[WRN]" )

#define LOG_ERR() \
        std::clog << LOG_FMT( "[ERR]" )

#endif /* LOGGER_HPP_ */
