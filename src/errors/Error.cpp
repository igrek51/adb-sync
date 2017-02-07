//
// Created by igrek on 07/02/17.
//

#include "Error.h"

Error::Error(const string& message) : message(message) {}

string Error::getMessage() {
    return message;
}