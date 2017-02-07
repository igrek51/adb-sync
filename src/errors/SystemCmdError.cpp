//
// Created by igrek on 07/02/17.
//

#include "SystemCmdError.h"
#include "../utils/string_utils.h"

SystemCmdError::SystemCmdError(string message) : Error(message) {
    errorCode = 0;
}

SystemCmdError::SystemCmdError(string message, int errorCode) : Error(message) {
    this->errorCode = errorCode;
}

string SystemCmdError::getMessage() {
    if (errorCode == 0) {
        return "failed executing command: " + message;
    } else {
        return "failed executing command (exit code " + itos(errorCode) + "): " + message;
    }
}

