//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_ADB_H
#define ADBSYNC_ADB_H

#include "FileSystem.h"

class ADB : FileSystem {
public:
    bool detectADB();

    bool detectDevice();

    virtual vector<File*>* listPath(string path);

};


#endif //ADBSYNC_ADB_H
