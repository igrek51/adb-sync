//
// Created by igrek on 07/02/17.
//

#include "ParseError.h"

ParseError::ParseError(string message) : Error(message) {

}

ParseError::~ParseError() {}

string ParseError::getMessage() {
    return "Parse error: " + message;
}



