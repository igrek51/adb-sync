//
// Created by igrek on 04/02/17.
//

#include "File.h"

File::File(string path, string name) {
    this->path = path;
    this->name = name;
}

File::~File() {}

string File::getName() {
    return name;
}

void File::setName(const string& name) {
    this->name = name;
}

string File::getPath() {
    return path;
}

void File::setPath(const string& path) {
    this->path = path;
}

string File::getFullPath() {
    return subfolder(path, name);
}

string File::subfolder(string path, string dirName) {
    if (path.empty()) {
        return dirName;
    }
    // append "/" bettween folders
    if (path.at(path.length() - 1) != '/') {
        path += '/';
    }
    return path + dirName;
}
