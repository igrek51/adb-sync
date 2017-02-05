//
// Created by igrek on 04/02/17.
//

#include "CmdExecutor.h"
#include "../logger/Logger.h"
#include "../errors/Error.h"
#include "../utils/string_utils.h"
#include <sstream>

bool CommandExecutor::executeWithEcho(string cmd) {
    Logger::debug("executing: " + cmd);

    if (system(cmd.c_str()) == 0)
        return true;
    Logger::error("Failed executing: " + cmd);
    return false;
}

bool CommandExecutor::execute(string cmd) {
    Logger::debug("executing: " + cmd);

    FILE* fp = popen(cmd.c_str(), "r");
    int errorCode = pclose(fp);
    if (errorCode == 0) {
        return true;
    }
    Logger::error("Failed executing: " + cmd);
    return false;
}

string CommandExecutor::executeAndRead(string cmd) {
    Logger::debug("executing: " + cmd);

    FILE* fp;
    char path[1035];
    /* Open the command for reading. */
    fp = popen(cmd.c_str(), "r");
    if (fp == NULL) {
        throw new Error("failed to run command: " + cmd);
    }

    stringstream ss;
    while (fgets(path, sizeof(path) - 1, fp) != NULL) {
        ss << path;
    }

    int errorCode = pclose(fp);
    if (errorCode != 0) {
        throw new Error("Failed to run command (" + itos(errorCode) + " error code): " + cmd);
    }

    return ss.str();
}
