#ifndef SYNCHRO_H
#define SYNCHRO_H

#include "task.h"

#include <iostream>
#include <vector>

using namespace std;

class SynchroPath {
public:
    SynchroPath(string source, string dest, bool content_check);
    string source;
    string dest;
    bool content_check;
};

string select_drive();

void dirlist_cmp(string head1name, string head2name, bool content_check, double prog_from, double prog_to);

#endif
