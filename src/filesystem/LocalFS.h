//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_LOCALFS_H
#define ADBSYNC_LOCALFS_H

#include "FileSystem.h"

class LocalFS : FileSystem {
public:
    virtual vector<File*>* listPath(string path);
};


#endif //ADBSYNC_LOCALFS_H
