//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_ERROR_H
#define ADBSYNC_ERROR_H

#include <string>

using namespace std;

class Error {
public:
    Error();

    Error(const string& message);

    virtual ~Error();

    virtual string getMessage();

protected:
    string message;
};


#endif //ADBSYNC_ERROR_H
