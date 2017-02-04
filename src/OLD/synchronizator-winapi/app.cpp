#include "app.h"

App* App::instance = NULL;

App* App::geti(){
    if(instance == NULL){
        instance = new App();
    }
    return instance;
}

App::App(){
    instance = this;
    historia = NULL;
    historia = new History();
    zadania = new vector<Task*>;
    filesearch = NULL;
}

App::~App(){
	if(Config::geti()->history_enabled){
        historia->truncate(Config::geti()->history_num);
		historia->save();
	}
    Config::geti()->save_config();
    IO::geti()->log("Sprz¹tanie...");
    if(FileSearch::active) delete filesearch;
    tasks_clear(zadania);
    delete historia;
	delete Controls::geti();
	IO::geti()->log("Goodbye World...");
    delete Config::geti();
    delete IO::geti();
	PostQuitMessage(0);
}
