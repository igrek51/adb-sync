//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_LOCALFS_H
#define ADBSYNC_LOCALFS_H

#include "FileSystem.h"
#include "Directory.h"
#include "RegularFile.h"

class LocalFS : FileSystem {
public:
    bool pathExists(string path);

    vector<File*>* listPath(string path);

private:
    File* parseLsOutput(string lsLine);

    Directory* parseLsDirectory(vector<string>* parts);

    RegularFile* parseLsRegularFile(vector<string>* parts);
};


#endif //ADBSYNC_LOCALFS_H
