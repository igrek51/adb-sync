#ifndef LOG_LEVEL_H
#define LOG_LEVEL_H

enum class LogLevel : int {
    OFF = 0, //no logs
    FATAL = 10, // FATAL
    ERROR = 20, // FATAL + ERROR
    WARN = 30, // FATAL + ERROR + WARN
    INFO = 40, // FATAL + ERROR + WARN + INFO
    DEBUG = 50, // FATAL + ERROR + WARN + INFO + DEBUG
    TRACE = 60,
    ALL = 1000, //all logs
};

#endif