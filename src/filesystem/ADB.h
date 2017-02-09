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

    bool pathExists(string path) override;

    vector<File*>* listPath(string path) override;

    virtual void saveModifyDate(RegularFile* file, boost::posix_time::ptime modifyDate) override;

protected:
    RegularFile* getRegularFileDetails(string path, string name) override;

    string escapePath(string path) override;

private:
    File* parseLsOutput(string path, string lsLine);

    Directory* parseLsDirectory(string path, vector<string>* parts);

    RegularFile* parseLsRegularFile(string path, vector<string>* parts);

    string busyboxPath;
};


#endif //ADBSYNC_ADB_H
