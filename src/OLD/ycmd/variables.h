#ifndef VARIABLES_H
#define VARIABLES_H

#include <iostream>
#include <vector>

using namespace std;

class Variable {
public:
    Variable(string name, string value);

    string name;
    string value;
};

vector<string> *get_variables_lines(string filename);

vector<Variable *> *get_variables(string filename);

string get_var_string(vector<Variable *> *variables, string name, string domyslny = "");

int get_var_int(vector<Variable *> *variables, string name, int domyslny = 0);

bool get_var_bool(vector<Variable *> *variables, string name, bool domyslny = false);

vector<string> *get_list_ex(string lista, string dir);

void add_to_set(vector<string> *kontener, string elem);

void remove_from_list(vector<string> *kontener, string elem);

#endif
