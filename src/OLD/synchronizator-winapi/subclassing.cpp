#include "app.h"

LRESULT CALLBACK App::subclass_wndproc_new(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	//odnalezienie kontrolki
    Control* kontrolka = NULL;
	for(unsigned int i=0; i<Controls::geti()->controls.size(); i++){
        if(Controls::geti()->controls.at(i)->handle == hwnd){
            kontrolka = Controls::geti()->controls.at(i);
            break;
        }
	}
    if(kontrolka==NULL){
        IO::geti()->error("Subclassing: Nie znaleziono kontrolki o podanym uchwycie");
        return 0;
    }
    string nazwa = kontrolka->name;
	//nowe procedury kontrolek

    //powrót do starej procedury
	return CallWindowProc(kontrolka->wndproc_old, hwnd, message, wParam, lParam);
}

void App::subclass(Control* kontrolka){
    if(kontrolka==NULL) return;
    kontrolka->wndproc_old = (WNDPROC) SetWindowLong(kontrolka->handle, GWL_WNDPROC, (LONG)wndproc_new);
}

void App::subclass(string name){
    subclass(Controls::geti()->find_control(name));
}

void App::un_subclass(Control* kontrolka){
    if(kontrolka==NULL) return;
	if(kontrolka->wndproc_old==NULL) return;
	SetWindowLong(kontrolka->handle, GWL_WNDPROC, (LONG)kontrolka->wndproc_old);
}

void App::un_subclass(string name){
    un_subclass(Controls::geti()->find_control(name));
}
