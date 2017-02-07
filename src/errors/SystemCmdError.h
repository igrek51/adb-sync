//
// Created by igrek on 07/02/17.
//

#ifndef ADBSYNC_SYSTEMCMDERROR_H
#define ADBSYNC_SYSTEMCMDERROR_H

#include "Error.h"

class SystemCmdError : public Error {
public:
    SystemCmdError(string cmd);

    SystemCmdError(string cmd, int errorCode);

    SystemCmdError(string cmd, int errorCode, string result);

    virtual ~SystemCmdError();

    virtual string getMessage();

protected:
    string cmd;
    int errorCode;
    string output;
};


#endif //ADBSYNC_SYSTEMCMDERROR_H
