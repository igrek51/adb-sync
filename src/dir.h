#ifndef DIR_H
#define DIR_H

#include <QDir>
#include <iostream>

using namespace std;

struct dir{
    string name;
    int size;
    dir *next;
};

bool dir_exists(string dirName_in);
void dirlist_add(dir **head, string name, int size);
void dirlist_destroy(dir **head);
int dirlist_num(dir *head);
char to_lower(char c);
bool str_cmp_lc(string s1, string s2);
dir* dirlist_search(dir *head, string name);

#endif // DIR_H
