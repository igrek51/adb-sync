//
// Created by igrek on 05/02/17.
//

#ifndef ADBSYNC_STRING_UTILS_H
#define ADBSYNC_STRING_UTILS_H

#include <string>
#include <vector>

using namespace std;

string trimSpaces(string s);

bool startsWith(string s, string prefix);

bool endsWith(string s, string suffix);

vector<string>* splitLines(string s);

vector<string>* splitByAny(string s, string delimiters);

/**
 * @param time time_t
 * @param pattern time pattern, example: %Y-%m-%d %H:%M:%S
 * @return time converted to string
 */
string time2string(time_t t, string pattern);

string replaceAll(string str, const string& from, const string& to);

string to_string(const char* s);

#endif //ADBSYNC_STRING_UTILS_H
