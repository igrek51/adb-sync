#ifndef LISTA_H
#define LISTA_H

#include <iostream>

using namespace std;

struct lista{
    string filename;
    string dir1;
    string dir2;
    int task_code; //1 - brak folderu, 2 - brak pliku, 3 - inny rozmiar pliku, 4 - inna wersja pliku, 5 - zbędny folder, 6 - zbędny plik
    lista *next;
};

void lista_add(lista **head, string filename, string dir1, string dir2, int task_code);
void lista_destroy(lista **head);
int lista_num(lista *head);
int get_task_num(lista *e);
string get_task_name(lista *e);
string* get_tasks(lista *e);
void lista_invert(lista *e);

#endif
