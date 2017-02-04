#include "app.h"
#include "strings.h"

int App::listbox_count(){
	return SendMessage(Controls::geti()->find("listbox"),LB_GETCOUNT,0,0);
}

void App::listbox_delete(int nr){
	SendMessage(Controls::geti()->find("listbox"),LB_DELETESTRING,nr,0);
}

void App::listbox_clear(){
	while(listbox_count()>0) listbox_delete(0);
}

void App::listbox_add(string s){
	SendMessage(Controls::geti()->find("listbox"),LB_ADDSTRING,NULL,(LPARAM)s.c_str());
}

int App::listbox_current(){
	int ret = SendMessage(Controls::geti()->find("listbox"),LB_GETCURSEL,0,0);
	if(ret==LB_ERR) return -1;
	return ret;
}

void App::listbox_select(int nr){
	if(nr>=listbox_count()) nr=-1;
	SendMessage(Controls::geti()->find("listbox"),LB_SETCURSEL,nr,0);
}

void App::show_lista(){
	listbox_clear();
	HDC dcList = GetDC(Controls::geti()->find("listbox"));
	SIZE textSize;
	HFONT hF = (HFONT)SendMessage(Controls::geti()->find("listbox"),WM_GETFONT,0,0);
	HGDIOBJ hOld = SelectObject(dcList,hF);
    stringstream ss;
    int max_w = 0;
    for(unsigned int i=0; i<zadania->size(); i++){
        ss_clear(ss);
        ss<<zadania->at(i)->name()<<": "<<zadania->at(i)->dir2<<"\\"<<zadania->at(i)->filename;
		listbox_add(ss.str());
        //d³ugoœæ tekstu w pixelach
		GetTextExtentPoint32(dcList,ss.str().c_str(),ss.str().length(),&textSize);
		if(textSize.cx > max_w) max_w = textSize.cx;
    }
	SelectObject(dcList,hOld);
	ReleaseDC(Controls::geti()->find("listbox"),dcList);
	SendMessage(Controls::geti()->find("listbox"),LB_SETHORIZONTALEXTENT,max_w,NULL);
}

void App::listbox_clicked(){
	int n = listbox_current();
	if(n==-1) return;
	if(zadania->size()==0) return;
    stringstream ss;
    Task* task = zadania->at(n);
    vector<string>* cmds = task->cmds();
    ss<<"Zadania dla elementu "<<task->dir2<<"\\"<<task->filename<<" ["<<cmds->size()<<"]:\r\n";
    for(unsigned int i=0; i<cmds->size(); i++){
        ss<<cmds->at(i);
        if(i<cmds->size()-1) ss<<"\r\n";
    }
    IO::geti()->echo(ss.str());
}
