//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_ERROR_H
#define ADBSYNC_ERROR_H

#include <string>

using namespace std;

class Error {
public:
    Error(const string& message);

    string message;
};


#endif //ADBSYNC_ERROR_H
