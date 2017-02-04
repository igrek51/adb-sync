#include "app.h"
#include "strings.h"
#include "files.h"

#include <commctrl.h>

void App::controls_fonts_set(){
    for(unsigned int i=0; i<Controls::geti()->controls.size(); i++){
        string fontface = Config::geti()->buttons_fontface;
        int fontsize = Config::geti()->buttons_fontsize;
        Controls::geti()->set_font(Controls::geti()->controls.at(i)->handle, fontface, fontsize);
    }
}

void App::text_vcenter(){
	int lines = SendMessage(Controls::geti()->find(Config::geti()->output_control),EM_GETLINECOUNT,0,0);
	int height = Config::geti()->buttons_fontsize*lines;
	int ypos = (50-height)/2;
	if(ypos<0) ypos=0;
	if(height>52) height=52;
    Controls::geti()->resize(Config::geti()->output_control, 0,ypos,Config::geti()->window_w,height);
}

void App::set_progress(double p){
	if(p>1) p=1;
	if(p<0) p=0;
	SendMessage(Controls::geti()->find("progress_bar"),PBM_SETPOS,(WPARAM)(p*10000),0);
}

void App::synchro_search(){
	set_progress(0);
	IO::geti()->echo("Wyszukiwanie dysku...");
	string drive = select_drive();
	if(drive.length()==0) return;
	if(zadania->size()>0) tasks_clear(zadania);
	//dla ka¿dego synchronizowanego katalogu
    stringstream ss;
	for(unsigned int i=0; i<Config::geti()->synchropaths.size(); i++){
        string source = Config::geti()->synchropaths.at(i)->source;
        string dest = drive+Config::geti()->synchropaths.at(i)->dest;
        double prog_from = double(i)/Config::geti()->synchropaths.size();
        double prog_to = double(i+1)/Config::geti()->synchropaths.size();
        ss_clear(ss);
		ss<<"Przeszukiwanie folderu \""<<dest<<"\"...";
		IO::geti()->echo(ss.str());
		dirlist_cmp(source, dest, Config::geti()->synchropaths.at(i)->content_check?true:false, prog_from, prog_to);
	}
	set_progress(1);
	ss_clear(ss);
	ss<<"Zakoñczono przeszukiwanie.\r\n";
	if(zadania->size()==0){
		ss<<"Brak elementów do synchronizacji.";
	}else{
		ss<<"Znalezione elementy synchronizacji: "<<zadania->size();
	}
	IO::geti()->echo(ss.str());
	show_lista();
}

void App::otworz_foldery(){
	int n = listbox_current();
	if(n==-1){
		IO::geti()->error("Nie wybrano elementu.");
		return;
	}
	if(zadania->size()==0) return;
    Task* task = zadania->at(n);
    ShellExecuteA(NULL,NULL,"explorer.exe",task->dir1.c_str(),NULL,SW_SHOW);
    ShellExecuteA(NULL,NULL,"explorer.exe",task->dir2.c_str(),NULL,SW_SHOW);
}

void App::viewer_open(string file){
    if(Config::geti()->external_viewer.length()==0){
        ShellExecuteA(NULL,"open",file.c_str(),NULL,NULL,SW_SHOW);
    }else{
        ShellExecuteA(NULL,"open",Config::geti()->external_viewer.c_str(),file.c_str(),NULL,SW_SHOW);
    }
}

void App::otworz_pliki(){
	int n = listbox_current();
	if(n==-1){
		IO::geti()->error("Nie wybrano elementu.");
		return;
	}
    if(zadania->size()==0) return;
    Task* task = zadania->at(n);
    if(task->code!=TASK_INNY_ROZMIAR && task->code!=TASK_INNA_WERSJA){
        IO::geti()->error("Funkcja niedostêpna dla tych elementów.");
        return;
    }
    viewer_open(task->dir1+"\\"+task->filename);
    viewer_open(task->dir2+"\\"+task->filename);
    files_cmp(task->dir1+"\\"+task->filename, task->dir2+"\\"+task->filename, true);
    Sleep(300);
    SetForegroundWindow(main_window);
}

void App::pb_usun(){
	if(FileSearch::active){
		IO::geti()->error("Trwa przeszukiwanie.");
		return;
	}
	int cr=listbox_current();
	if(cr==-1){
		IO::geti()->error("Nie wybrano elementu.");
		return;
	}
    if(zadania->size()==0) return;
    zadania->erase(zadania->begin() + cr);
	show_lista();
    IO::geti()->echo("Usuniêto element synchronizacji z listy.");
}

void App::pb_odwroc(){
	int n = listbox_current();
	if(n==-1){
		IO::geti()->error("Nie wybrano elementu.");
		return;
	}
	if(zadania->size()==0) return;
    Task* task = zadania->at(n);
    task->invert();
    show_lista();
    listbox_select(n);
    IO::geti()->echo("Odwrócono kierunek synchronizacji dla wybranego elementu.");
}

void App::wykonaj_1(){
	if(FileSearch::active){
		IO::geti()->error("Trwa przeszukiwanie.");
		return;
	}
	int cr=listbox_current();
	if(cr==-1){
		IO::geti()->error("Nie wybrano elementu.");
		return;
	}
    if(zadania->size()==0) return;
    zadania->at(cr)->execute();
    zadania->erase(zadania->begin() + cr);
	show_lista();
	listbox_select(cr);
    IO::geti()->echo("Wykonano zadania 1 elementu synchronizacji.");
}

void App::wykonaj_wszystko(){
	if(FileSearch::active){
		IO::geti()->error("Trwa przeszukiwanie.");
		return;
	}
	IO::geti()->echo("Wykonywanie zadañ...");
	if(zadania->size()==0){
		IO::geti()->error("Lista zadañ jest pusta.");
		return;
	}
    execute_all_tasks(zadania);
    tasks_clear(zadania);
	show_lista();
	IO::geti()->echo("Zakoñczono synchronizacjê.");
}
