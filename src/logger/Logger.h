//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_LOGGER_H
#define ADBSYNC_LOGGER_H

#include <iostream>
#include "LogLevel.h"
#include "../errors/Error.h"

using namespace std;

class Logger {

public:

    static void fatal(string s);

    static void error(string s);

    static void error(Error* error);

    static void warn(string s);

    static void info(string s);

    static void debug(string s);

    static const string C_RESET;
    static const string C_BOLD;
    static const string C_DIM;
    static const string C_ITALIC;
    static const string C_UNDERLINE;

    static const string C_BLACK ;
    static const string C_RED ;
    static const string C_GREEN ;
    static const string C_YELLOW ;
    static const string C_BLUE  ;
    static const string C_MAGENTA ;
    static const string C_CYAN  ;
    static const string C_WHITE ;

    static const LogLevel ECHO_LEVEL = LogLevel::ALL;

    static void log(string s, LogLevel level);

private:
    static volatile bool LOCK;

};


#endif //ADBSYNC_LOGGER_H
