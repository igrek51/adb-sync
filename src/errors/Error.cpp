//
// Created by igrek on 07/02/17.
//

#include "Error.h"

Error::Error() {}

Error::Error(const string& message) : message(message) {}

Error::~Error() {}

string Error::getMessage() {
	return message;
}

