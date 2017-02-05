//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_COMMANDEXECUTOR_H
#define ADBSYNC_COMMANDEXECUTOR_H

#include <string>

using namespace std;

class CommandExecutor {
public:
    static bool executeWithEcho(string cmd);

    static bool execute(string cmd);

    static string executeAndRead(string cmd);
};


#endif //ADBSYNC_COMMANDEXECUTOR_H
