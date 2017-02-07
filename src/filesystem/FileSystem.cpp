//
// Created by igrek on 04/02/17.
//

#include "FileSystem.h"
#include "../errors/ParseError.h"

string nextNonemptyPart(vector<string>* parts, unsigned int& index) {
    for (; index < parts->size(); index++) {
        string& part = parts->at(index);
        if (!part.empty()) {
            index++;
            return part;
        }
    }
    return "";
}

time_t parseLsTime(string date, string pattern) {
    struct tm tm;
    if (strptime(date.c_str(), pattern.c_str(), &tm) == NULL) {
        throw new ParseError("invalid date: " + date);
    }
    time_t tim = mktime(&tm);
    if (tim == -1) {
        throw new ParseError("mktime error for date: " + date);
    }
    return tim;
}