//
// Created by igrek on 04/02/17.
//

#include "FileSystem.h"
#include "../errors/ParseError.h"
#include "../utils/string_utils.h"

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

boost::posix_time::ptime FileSystem::parseLsTime(string date, string pattern) {
    boost::posix_time::ptime t = string2time(date, pattern);
    if (t == boost::posix_time::ptime()) {
        throw new ParseError("invalid date: " + date);
    }
    return t;
}
