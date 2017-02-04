#ifndef APP_H
#define APP_H

#include "config.h"
#include "controls.h"
#include "io.h"
#include "history.h"
#include "thread.h"

#include <windows.h>
#include <iostream>

using namespace std;

class App{
private:
    static App* instance;
    App();
public:
    static App* geti();
	~App();
    HWND main_window;
	HINSTANCE* hInst;
	//Events
	void event_init(HWND *window);
	void event_button(WPARAM wParam, LPARAM lParam);
	void event_resize();
	void event_move();
    //historia
    History* historia;
    //zadania
    vector<Task*>* zadania;
    void exec_cmd(string l);
    void execute_all_tasks(vector<Task*>* zadania);
	//program
    void controls_fonts_set();
    void text_vcenter();
	void set_progress(double p);
	void synchro_search();
	void otworz_foldery();
    void viewer_open(string file);
	void otworz_pliki();
	void pb_usun();
	void pb_odwroc();
	void wykonaj_1();
	void wykonaj_wszystko();
	//listbox
	void show_lista();
	int listbox_count();
	void listbox_delete(int nr);
	void listbox_clear();
	void listbox_add(string s);
	int listbox_current();
	void listbox_select(int nr);
	void listbox_clicked();
	//subclassing
    WNDPROC windowProc, wndproc_new;
    LRESULT CALLBACK subclass_wndproc_new(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    void subclass(Control* kontrolka);
    void subclass(string name);
    void un_subclass(Control* kontrolka);
    void un_subclass(string name);
	//w¹tki
    void filesearch_start();
    FileSearch* filesearch;
};

#endif
