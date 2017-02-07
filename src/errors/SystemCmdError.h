//
// Created by igrek on 07/02/17.
//

#ifndef ADBSYNC_SYSTEMCMDERROR_H
#define ADBSYNC_SYSTEMCMDERROR_H

#include "Error.h"

class SystemCmdError : public Error {
public:
    SystemCmdError(string message);

    SystemCmdError(string message, int errorCode);

    virtual string getMessage();

protected:
    int errorCode;
};


#endif //ADBSYNC_SYSTEMCMDERROR_H
