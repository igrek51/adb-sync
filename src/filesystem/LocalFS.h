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
    bool pathExists(string path) override;

    vector<File*>* listPath(string path) override;

	virtual void saveModifyDate(string filePath, time_t modifyDate) override;

	void mkdir(string path);

protected:
    RegularFile* getRegularFileDetails(string path, string name) override;

	string escapePath(string path);

private:
    File* parseLsOutput(string path, string lsLine);

    Directory* parseLsDirectory(string path, vector<string>* parts);

    RegularFile* parseLsRegularFile(string path, vector<string>* parts);
};


#endif //ADBSYNC_LOCALFS_H
