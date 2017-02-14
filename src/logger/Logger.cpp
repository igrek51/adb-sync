//
// Created by igrek on 04/02/17.
//

#include "Logger.h"

//TODO mutexy

const string Logger::C_RESET = "\033[0m";
const string Logger::C_BOLD = "\033[1m";
const string Logger::C_DIM = "\033[2m";
const string Logger::C_ITALIC = "\033[3m";
const string Logger::C_UNDERLINE = "\033[4m";

const string Logger::C_BLACK = "\033[30m";
const string Logger::C_RED = "\033[31m";
const string Logger::C_GREEN = "\033[32m";
const string Logger::C_YELLOW = "\033[33m";
const string Logger::C_BLUE  = "\033[34m";
const string Logger::C_MAGENTA = "\033[35m";
const string Logger::C_CYAN  = "\033[36m";
const string Logger::C_WHITE = "\033[37m";

volatile bool Logger::LOCK = false;

void Logger::fatal(string s) {
    string s2 = C_BOLD + C_RED + "[FATAL] " + C_RESET + s;
    log(s2, LogLevel::FATAL);
    exit(1);
}

void Logger::error(string s) {
    string s2 = C_BOLD + C_RED + "[ERROR] " + C_RESET + s;
    log(s2, LogLevel::ERROR);
}

void Logger::error(Error* error) {
    string s2 = C_BOLD + C_RED + "[ERROR] " + C_RESET + error->getMessage();
    delete error;
    log(s2, LogLevel::ERROR);
}

void Logger::warn(string s) {
    string s2 = C_BOLD + C_YELLOW + "[warn] " + C_RESET + s;
    log(s2, LogLevel::WARN);
}

void Logger::info(string s) {
    string s2 = C_BOLD + C_BLUE + "[info] " + C_RESET + s;
    log(s2, LogLevel::INFO);
}

void Logger::debug(string s) {
    string s2 = C_BOLD + C_GREEN + "[debug] " + C_RESET + s;
    log(s2, LogLevel::DEBUG);
}

void Logger::log(string s, LogLevel level) {
    if (level > ECHO_LEVEL) return;
    while(LOCK){
        continue;
    }
    LOCK = true;
    cout << s << endl;
    LOCK = false;
}
