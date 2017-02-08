//
// Created by igrek on 05/02/17.
//

#ifndef ADBSYNC_STRING_UTILS_H
#define ADBSYNC_STRING_UTILS_H

#include <string>
#include <vector>

using namespace std;

string trimSpaces(string s);

string itos(int number);

bool startsWith(string s, string prefix);

bool endsWith(string s, string suffix);

vector<string>* splitLines(string s);

vector<string>* splitByAny(string s, string delimiters);

string toString(int i);

string toString(size_t i);

string toString(bool b);

string replaceAll(string str, const string& from, const string& to);

#endif //ADBSYNC_STRING_UTILS_H
