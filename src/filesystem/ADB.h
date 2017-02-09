//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_ADB_H
#define ADBSYNC_ADB_H

#include "FileSystem.h"
#include "Directory.h"
#include "RegularFile.h"

class ADB : FileSystem {
public:
    ADB();

    void testADB();

    void detectDevice();

    void checkBusyBox();

    string shell(string cmd);

    bool pathExists(string path);

    vector<File*>* listPath(string path);

private:
    File* parseLsOutput(string lsLine);

    Directory* parseLsDirectory(vector<string>* parts);

    RegularFile* parseLsRegularFile(vector<string>* parts);

    string escapePath(string path);

    string busyboxPath;
};


#endif //ADBSYNC_ADB_H
