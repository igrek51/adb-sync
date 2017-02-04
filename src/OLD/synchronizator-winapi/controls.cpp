#include "controls.h"
#include "io.h"
#include "app.h"

#include <commctrl.h>
#include <richedit.h>

Controls* Controls::instance = NULL;

Controls* Controls::geti(){
    if(instance == NULL){
        instance = new Controls();
    }
    return instance;
}

Controls::Controls(){
    instance = this;
}

Controls::~Controls(){
    for(unsigned int i=0; i<controls.size(); i++){
        delete controls.at(i);
    }
    controls.clear();
}


Control::Control(HWND handle, string name){
    this->handle = handle;
    this->name = name;
    wndproc_old = NULL;
}

Control::~Control(){
    App::geti()->un_subclass(this);
    if(this->handle!=NULL) DestroyWindow(this->handle);
}


Control* Controls::find_control(string name){
    if(name.length()==0){
        IO::geti()->error("Wyszukiwanie kontrolki o pustej nazwie");
        return NULL;
    }
    for(unsigned int i=0; i<controls.size(); i++){
        if(controls.at(i)->name == name){
            return controls.at(i);
        }
    }
    IO::geti()->error("Nie odnaleziono kontrolki o nazwie: "+name);
    return NULL;
}

HWND Controls::find(string name){
    Control* kontrolka = find_control(name);
    if(kontrolka==NULL) return NULL;
    return kontrolka->handle;
}

bool Controls::exists(string name){
    if(name.length()==0) return false;
    for(unsigned int i=0; i<controls.size(); i++){
        if(controls.at(i)->name == name) return true;
    }
    return false;
}

string Controls::get_button_name(int button_nr){
    //jeœli numer jest poprawny
    if(button_nr>=1 && button_nr<=(int)controls.size()){
        return controls.at(button_nr-1)->name;
    }
    stringstream ss;
    ss<<"Nie odnaleziono kontrolki o numerze: "<<button_nr;
    IO::geti()->error(ss.str());
    return "";
}

void Controls::create_button(string text, int x, int y, int w, int h, string name){
    int button_nr = controls.size() + 1;
    HWND handle = CreateWindowEx(0, WC_BUTTON, text.c_str(), WS_CHILD|WS_VISIBLE, x, y, w, h, App::geti()->main_window, (HMENU)button_nr, *App::geti()->hInst, 0);
    controls.push_back(new Control(handle, name));
}

void Controls::create_button_multiline(string text, int x, int y, int w, int h, string name){
    int button_nr = controls.size() + 1;
    HWND handle = CreateWindowEx(0, WC_BUTTON, text.c_str(), WS_CHILD|WS_VISIBLE|BS_MULTILINE, x, y, w, h, App::geti()->main_window, (HMENU)button_nr, *App::geti()->hInst, 0);
    controls.push_back(new Control(handle, name));
}

void Controls::create_edit(string text, int x, int y, int w, int h, string name){
	HWND handle = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, text.c_str(), WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, x, y, w, h, App::geti()->main_window, 0, *App::geti()->hInst, 0);
    controls.push_back(new Control(handle, name));
}

void Controls::create_edit_center(string text, int x, int y, int w, int h, string name){
	HWND handle = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, text.c_str(), WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL|ES_CENTER, x, y, w, h, App::geti()->main_window, 0, *App::geti()->hInst, 0);
    controls.push_back(new Control(handle, name));
}

void Controls::create_static(string text, int x, int y, int w, int h, string name){
	HWND handle = CreateWindowEx(0, WC_STATIC, text.c_str(), WS_CHILD|WS_VISIBLE, x, y, w, h, App::geti()->main_window, 0, *App::geti()->hInst, 0);
    controls.push_back(new Control(handle, name));
}

void Controls::create_static_center(string text, int x, int y, int w, int h, string name){
	HWND handle = CreateWindowEx(0, WC_STATIC, text.c_str(), WS_CHILD|WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE, x, y, w, h, App::geti()->main_window, 0, *App::geti()->hInst, 0);
    controls.push_back(new Control(handle, name));
}

void Controls::create_groupbox(string text, int x, int y, int w, int h){
    HWND handle = CreateWindowEx(0, WC_BUTTON, text.c_str(), WS_CHILD|WS_VISIBLE|BS_GROUPBOX, x, y, w, h, App::geti()->main_window, 0, *App::geti()->hInst, 0);
    controls.push_back(new Control(handle, ""));
}


void Controls::set_text(string control_name, string text){
    HWND uchwyt = find(control_name);
    if(uchwyt==NULL) return;
    SetWindowText(uchwyt, text.c_str());
}

void Controls::set_text(string control_name, int number){
    stringstream ss;
    ss<<number;
    set_text(control_name, ss.str());
}

string Controls::get_text(string control_name){
    Control* kontrolka = find_control(control_name);
    if(kontrolka==NULL) return "";
    if(kontrolka->handle==NULL) return "";
    char *str2 = new char[512];
	GetWindowText(kontrolka->handle, str2, 512);
    string result = str2;
	delete[] str2;
    return result;
}

int Controls::get_int(string control_name){
    string content = get_text(control_name);
    if(content.length()==0) return 0;
    return atoi(content.c_str());
}

void Controls::select_all(string control_name){
    Control* kontrolka = find_control(control_name);
    if(kontrolka==NULL) return;
    if(kontrolka->handle==NULL) return;
    SendMessage(kontrolka->handle, EM_SETSEL, 0, -1);
}

void Controls::set_focus(string control_name){
    Control* kontrolka = find_control(control_name);
    if(kontrolka==NULL) return;
    if(kontrolka->handle==NULL) return;
    SetFocus(kontrolka->handle);
}


void Controls::resize(string control_name, int x, int y, int w, int h){
    Control* kontrolka = find_control(control_name);
    if(kontrolka==NULL) return;
    if(kontrolka->handle==NULL) return;
    unsigned int flag = 0;
    if(x==-1 && y==-1) flag = SWP_NOMOVE;
    if(w==-1 && h==-1) flag = SWP_NOSIZE;
    SetWindowPos(kontrolka->handle, HWND_TOP, x, y, w, h, flag);
}


void Controls::set_font(HWND kontrolka, string fontface, int fontsize){
    if(kontrolka==NULL) return;
    HFONT hFont = CreateFont(fontsize, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, fontface.c_str());
	SendMessage(kontrolka, WM_SETFONT, (WPARAM)hFont, true);
}

void Controls::set_font(string name, string fontface, int fontsize){
    set_font(find(name), fontface, fontsize);
}
