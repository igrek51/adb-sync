//
// Created by igrek on 05/02/17.
//

#include "string_utils.h"

#include <sstream>
#include <boost/algorithm/string.hpp>

string trimSpaces(string s) {
    //delete spaces from the end
    while (s.length() > 0 && s[s.length() - 1] == ' ') {
        s = s.substr(0, s.length() - 1);
    }
    //delete spaces from the beginning
    while (s.length() > 0 && s[0] == ' ') {
        s = s.substr(1);
    }
    return s;
}

string itos(int number) {
    stringstream ss;
    ss << number;
    return ss.str();
}

string toString(int i) {
    return itos(i);
}

string toString(size_t i) {
    return itos(i);
}

string toString(bool b) {
    return b ? "true" : "false";
}

bool startsWith(string s, string prefix) {
    if (prefix.length() == 0) return true;
    if (prefix.length() > s.length()) return false;
    s = s.substr(0, prefix.length());
    return s == prefix;
}

bool endsWith(string s, string suffix) {
    if (suffix.length() == 0) return true;
    if (suffix.length() > s.length()) return false;
    s = s.substr(s.length() - suffix.length(), suffix.length());
    return s == suffix;
}

vector<string>* splitLines(string s) {
    vector<string>* strs = new vector<string>();
    boost::split(*strs, s, boost::is_any_of("\r\n"));
    return strs;
}

vector<string>* splitByAny(string s, string delimiters) {
    vector<string>* strs = new vector<string>();
    boost::split(*strs, s, boost::is_any_of(delimiters));
    return strs;
}