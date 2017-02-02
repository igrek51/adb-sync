#ifndef HISTORY_H
#define HISTORY_H

#include <iostream>

using namespace std;

struct history{
	string txt;
	history *next;
};

void history_add(history **head, string txt);
void history_add_time(history **head, string txt);
void history_destroy(history **head);
void history_delete1(history **head);
int history_num(history *head);
void history_load(history **head);
void history_save(history **head);
void history_truncate(history **head, int nr);

#endif
