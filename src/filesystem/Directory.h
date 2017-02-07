//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_DIRECTORY_H
#define ADBSYNC_DIRECTORY_H

#include "File.h"
#include "RegularFile.h"
#include <vector>

class Directory : public File {
public:
    Directory(string name);

    ~Directory();

    vector<File*>* getAllFiles();

    vector<RegularFile*>* getRegularFiles();

    vector<Directory*>* getDirectories();

    void addFile(File* file);

    void addFiles(vector<File*>* filesList);

private:
    vector<File*>* files;
};


#endif //ADBSYNC_DIRECTORY_H
