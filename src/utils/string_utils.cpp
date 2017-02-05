//
// Created by igrek on 05/02/17.
//

#include "string_utils.h"

#include <sstream>

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

bool beginsWith(string s, string prefix) {
    if (prefix.length() == 0) return true;
    if (prefix.length() > s.length()) return false;
    s = s.substr(0, prefix.length());
    return s == prefix;
}