//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_COMMANDEXECUTOR_H
#define ADBSYNC_COMMANDEXECUTOR_H

#include <string>
#include "../errors/SystemCmdError.h"

using namespace std;

class CommandExecutor {
public:
    static void executeVerbose(string cmd);

    static void execute(string cmd);

    static string executeAndRead(string cmd);
};


#endif //ADBSYNC_COMMANDEXECUTOR_H
