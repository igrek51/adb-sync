//
// Created by igrek on 04/02/17.
//

#include "FileSystem.h"

FileSystem::FileSystem() {}

FileSystem::~FileSystem() {}

string FileSystem::nextNonemptyPart(vector<string>* parts, unsigned int& index) {
    for (; index < parts->size(); index++) {
        string& part = parts->at(index);
        if (!part.empty()) {
            index++;
            return part;
        }
    }
    return "";
}