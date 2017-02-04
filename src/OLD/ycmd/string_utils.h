#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <iostream>
#include <vector>

using namespace std;

string trim_spaces(string s);

string trim_quotes(string s);

vector<string> *get_param_list(string lista);

bool ends_with(string name, string ext);

string replaceStr(string str, char find, char replaceTo);

string time_interval_string(time_t interval_s);

#endif
