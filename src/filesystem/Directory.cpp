//
// Created by igrek on 04/02/17.
//

#include "Directory.h"

Directory::Directory(string name) {
    this->name = name;
    files = new vector<File*>();
}

Directory::~Directory() {
    for(File* file : *files){
        delete file;
    }
    files->clear();
    delete files;
}

vector<File*>* Directory::getAllFiles() {
    return files;
}

void Directory::addFile(File* file) {
    files->push_back(file);
}

vector<RegularFile*>* Directory::getRegularFiles() {
    vector<RegularFile*>* list = new vector<RegularFile*>();
    for(File* file : *files){
        if(dynamic_cast<RegularFile*>(file) != nullptr){ // instance of RegularFile
            RegularFile* casted = dynamic_cast<RegularFile*>(file);
            list->push_back(casted);
        }
    }
    return list;
}

vector<Directory*>* Directory::getDirectories() {
    vector<Directory*>* list = new vector<Directory*>();
    for(File* file : *files){
        if(dynamic_cast<Directory*>(file) != nullptr){ // instance of Directory
            Directory* casted = dynamic_cast<Directory*>(file);
            list->push_back(casted);
        }
    }
    return list;
}

void Directory::addFiles(vector<File*>* filesList) {
    for (File* file : *filesList) {
        addFile(file);
    }
}
