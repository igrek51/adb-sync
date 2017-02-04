#include "task.h"
#include "app.h"

Task::Task(string filename, string dir1, string dir2, int code){
    this->filename = filename;
    this->dir1 = dir1;
    this->dir2 = dir2;
    this->code = code;
}

string Task::name(){
    if(code==TASK_BRAK_FOLDERU) return "Brak folderu";
	if(code==TASK_BRAK_PLIKU) return "Brak pliku";
	if(code==TASK_INNY_ROZMIAR) return "Inny rozmiar pliku";
	if(code==TASK_INNA_WERSJA) return "Inna wersja pliku";
	if(code==TASK_ZBEDNY_FOLDER) return "Zbêdny folder";
	if(code==TASK_ZBEDNY_PLIK) return "Zbêdny plik";
	return "";
}

vector<string>* Task::cmds(){
    vector<string>* cmdsy = new vector<string>;
    switch(code){
		case TASK_BRAK_FOLDERU:{
			cmdsy->push_back("md \""+dir1+"\\"+filename+"\"");
			cmdsy->push_back("xcopy \""+dir1+"\\"+filename+"\\*.*\" \""+dir2+"\\"+filename+"\\*.*\" /e /c");
		}break;
		case TASK_BRAK_PLIKU:{
			cmdsy->push_back("copy \""+dir1+"\\"+filename+"\" \""+dir2+"\\"+filename+"\"");
		}break;
		case TASK_INNY_ROZMIAR:{
			cmdsy->push_back("del \""+dir2+"\\"+filename+"\"");
			cmdsy->push_back("copy \""+dir1+"\\"+filename+"\" \""+dir2+"\\"+filename+"\"");
		}break;
		case TASK_INNA_WERSJA:{
			cmdsy->push_back("del \""+dir2+"\\"+filename+"\"");
			cmdsy->push_back("copy \""+dir1+"\\"+filename+"\" \""+dir2+"\\"+filename+"\"");
		}break;
		case TASK_ZBEDNY_FOLDER:{
			cmdsy->push_back("rd \""+dir2+"\\"+filename+"\" /s /q");
		}break;
		case TASK_ZBEDNY_PLIK:{
			cmdsy->push_back("del \""+dir2+"\\"+filename+"\"");
		}break;
	}
    return cmdsy;
}

int Task::cmds_number(){
    vector<string>* cmdsy = cmds();
    int number = cmdsy->size();
    delete cmdsy;
    return number;
}

void Task::invert(){
    string temp = dir1;
	dir1 = dir2;
	dir2 = temp;
	if(code==TASK_BRAK_FOLDERU){
		code=TASK_ZBEDNY_FOLDER;
	}else if(code==TASK_BRAK_PLIKU){
		code=TASK_ZBEDNY_PLIK;
	}else if(code==TASK_ZBEDNY_FOLDER){
		code=TASK_BRAK_FOLDERU;
	}else if(code==TASK_ZBEDNY_PLIK){
		code=TASK_BRAK_PLIKU;
	}
}

void Task::execute(){
    vector<string>* cmdsy = cmds();
	for(unsigned int i=0; i<cmdsy->size(); i++){
		App::geti()->exec_cmd(cmdsy->at(i));
	}
    delete cmdsy;
}

void add_task(string filename, string dir1, string dir2, int code){
    App::geti()->zadania->push_back(new Task(filename, dir1, dir2, code));
    App::geti()->show_lista();
}

void tasks_clear(vector<Task*>* tasks){
    for(unsigned int i=0; i<tasks->size(); i++){
        delete tasks->at(i);
    }
    tasks->clear();
}

void App::exec_cmd(string l){
	IO::geti()->log("Wykonywanie: "+l);
    SHELLEXECUTEINFO ShExecInfo = {0};
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = main_window;
    ShExecInfo.lpVerb = "open";
    ShExecInfo.lpFile = "cmd.exe";
    ShExecInfo.lpParameters = ("/c \""+l+"\"").c_str();
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = SW_HIDE;
    ShExecInfo.hInstApp = NULL;
    if(!ShellExecuteEx(&ShExecInfo)){
        IO::geti()->error("Blad polecenia: "+l);
        return;
    }
    WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
    historia->add_with_time(l);
    /*
    if(system(l.c_str())==0){
        historia->add_with_time(l);
    }else{
        IO::geti()->error("Blad polecenia: "+l);
    }
    */
}

void App::execute_all_tasks(vector<Task*>* zadania){
    for(unsigned int i=0; i<zadania->size(); i++){
        set_progress(double(i)/zadania->size());
		zadania->at(i)->execute();
	}
	set_progress(1);
}


