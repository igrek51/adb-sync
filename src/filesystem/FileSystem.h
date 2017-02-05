//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_FILESYSTEM_H
#define ADBSYNC_FILESYSTEM_H

#include <string>
#include <vector>
#include "File.h"

using namespace std;

class FileSystem {
public:
    virtual vector<File*>* listPath(string path) = 0;
};


#endif //ADBSYNC_FILESYSTEM_H
