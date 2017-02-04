#ifndef HISTORY_H
#define HISTORY_H

#include <iostream>
#include <vector>

using namespace std;

class History {
public:
    History();
    ~History();
    vector<string>* history;
    void add_with_time(string txt);
    void load();
    bool save();
    void truncate(unsigned int remains);
};

string get_time();

#endif
