#include "history.h"
#include "config.h"
#include "files.h"
#include "io.h"

#include <fstream>
#include <ctime>
#include <sstream>

History::History(){
    history = new vector<string>;
}

History::~History(){
    delete history;
}

void History::add_with_time(string txt){
    stringstream ss;
    ss<<get_time()<<" - "<<txt;
    history->push_back(ss.str());
}

void History::load(){
    IO::geti()->log("Odczytywanie historii poleceñ...");
    history = get_nonempty_lines(Config::geti()->history_filename);
}

bool History::save(){
    IO::geti()->log("Zapisywanie historii poleceñ...");
    stringstream ss;
    for(unsigned int i=0; i<history->size(); i++){
        ss<<history->at(i)<<endl;
    }
    return save_file(Config::geti()->history_filename, ss.str());
}

void History::truncate(unsigned int remains){
    stringstream ss;
    ss<<"Usuwanie starej historii poleceñ (liczba poleceñ: "<<history->size()<<", ograniczenie: "<<remains<<")";
    IO::geti()->log(ss.str());
    while(history->size() > remains){
        history->erase(history->begin());
    }
}

