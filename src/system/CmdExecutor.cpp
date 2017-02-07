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
        throw new SystemCmdError(cmd, errorCode);
    }
}

string CommandExecutor::executeAndRead(string cmd) {
    Logger::debug("executing: " + cmd);

    char buffer[128];
    stringstream result;
    // stderr to stdout
    FILE* pipe = popen((cmd + " 2>&1").c_str(), "r");
    if (!pipe) throw new SystemCmdError(cmd);
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result << buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }

    int errorCode = pclose(pipe);
    if (errorCode != 0) {
        throw new SystemCmdError(cmd, errorCode, result.str());
    }

    return result.str();
}
