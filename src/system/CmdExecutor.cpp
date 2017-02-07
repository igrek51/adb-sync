//
// Created by igrek on 04/02/17.
//

#include "CmdExecutor.h"
#include "../logger/Logger.h"
#include <sstream>

void CommandExecutor::executeVerbose(string cmd) {
    Logger::debug("executing: " + cmd);

    if (system(cmd.c_str()) != 0)
        throw new SystemCmdError(cmd);
}

void CommandExecutor::execute(string cmd) {
    Logger::debug("executing: " + cmd);

    FILE* fp = popen(cmd.c_str(), "r");
    int errorCode = pclose(fp);
    if (errorCode != 0) {
        throw new SystemCmdError(cmd);
    }
}

string CommandExecutor::executeAndRead(string cmd) {
    Logger::debug("executing: " + cmd);

    FILE* fp;
    char path[1035];
    fp = popen(cmd.c_str(), "r");
    if (fp == NULL) {
        throw new SystemCmdError(cmd);
    }

    stringstream ss;
    while (fgets(path, sizeof(path) - 1, fp) != NULL) {
        ss << path;
    }

    int errorCode = pclose(fp);
    if (errorCode != 0) {
        throw new SystemCmdError(cmd, errorCode);
    }

    return ss.str();
}
