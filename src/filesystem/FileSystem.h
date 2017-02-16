//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_FILESYSTEM_H
#define ADBSYNC_FILESYSTEM_H

#include <string>
#include <vector>
#include "File.h"
#include "Directory.h"

using namespace std;

class FileSystem {
public:
    FileSystem();

    virtual ~FileSystem();

    virtual bool pathExists(string path) = 0;

    virtual vector<File*>* listPath(string path) = 0;

	virtual void saveModifyDate(RegularFile* file, time_t modifyDate) = 0;

protected:
    virtual RegularFile* getRegularFileDetails(string path, string name) = 0;

    virtual string escapePath(string path) = 0;

    string nextNonemptyPart(vector<string>* parts, unsigned int& index);
};


#endif //ADBSYNC_FILESYSTEM_H
