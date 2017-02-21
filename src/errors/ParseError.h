//
// Created by igrek on 07/02/17.
//

#ifndef ADBSYNC_PARSEERROR_H
#define ADBSYNC_PARSEERROR_H

#include "Error.h"

class ParseError : public Error {
public:
	ParseError(string message);

	virtual ~ParseError();

	virtual string getMessage();

protected:
};


#endif //ADBSYNC_PARSEERROR_H
