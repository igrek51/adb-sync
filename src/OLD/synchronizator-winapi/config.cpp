#include "config.h"
#include "files.h"
#include "io.h"
#include "strings.h"
#include "version.h"
#include "app.h"

#include <cstdlib>

Config* Config::instance = NULL;

Config* Config::geti(){
    if(instance == NULL){
        instance = new Config();
    }
    return instance;
}

Config::Config(){
    instance = this;
    //ustawienia domyœlne
    program_name = "Igrek Synchronizator";
    config_filename = "conf.ini";
    log_filename = "log.txt";
    history_filename = "history.txt";
    buttons_fontface = "MS Shell Dlg 2";
    buttons_fontsize = 13;
    version = VERSION;
    version += " (win)";
    output_control = "info";
    //  Zmienne
    window_w = 560;
    window_h = 380;
    history_num = 50;
    //  z pliku konfiguracyjnego
    save_wnd_pos = 0;
    wnd_pos_x = 0;
    wnd_pos_y = 0;
    save_wnd_size = 0;
    log_enabled = true;
    history_enabled = true;
    synchro_paths_num = 0;
    external_viewer = "";
}

ConfigVariable::ConfigVariable(string name, string value){
    this->name = name;
    this->value = value;
}

void Config::load_config(){
    if(!file_exists(config_filename)){
        IO::geti()->message_box("B³¹d", "Brak pliku konfiguracyjnego - ³adowanie wartoœci domyœlnych");
        return;
    }
    IO::geti()->log("Wczytywanie ustawieñ z pliku konfiguracyjnego...");
    vector<ConfigVariable*>* variables = get_config_variables(config_filename);
    RECT wnd_rect;
    //odczyt zmiennych
    save_wnd_pos = get_config_int(variables, "save_wnd_pos", 0);
    if(save_wnd_pos == 2){ //reset ustawieñ
		GetWindowRect(App::geti()->main_window, &wnd_rect);
		wnd_pos_x = wnd_rect.left;
		wnd_pos_y = wnd_rect.top;
		save_wnd_pos = 1;
	}else if(save_wnd_pos==1){
		wnd_pos_x = get_config_int(variables, "wnd_pos_x");
		wnd_pos_y = get_config_int(variables, "wnd_pos_y");
	}else{
		save_wnd_pos = 0;
	}
	//zapisany rozmiar okna
	save_wnd_size = get_config_int(variables, "save_wnd_size", 0);
	if(save_wnd_size==2){
		GetWindowRect(App::geti()->main_window, &wnd_rect);
		window_w = wnd_rect.right-wnd_rect.left;
		window_h = wnd_rect.bottom-wnd_rect.top;
		save_wnd_size = 1;
	}else if(save_wnd_size==1){
        window_w = get_config_int(variables, "window_w", window_w);
		window_h = get_config_int(variables, "window_h", window_h);
	}else{
		save_wnd_size = 0;
	}
	//liczba katalogów do synchronizacji
	synchro_paths_num = get_config_int(variables, "synchro_paths_num", 0);
    stringstream ss;
	for(int i=0; i<synchro_paths_num; i++){
		ss_clear(ss);
		ss<<"synchro_path_"<<i+1<<"_source";
        string source = get_config_string(variables, ss.str());
		ss_clear(ss);
		ss<<"synchro_path_"<<i+1<<"_dest";
        string dest = get_config_string(variables, ss.str());
		ss_clear(ss);
		ss<<"synchro_path_"<<i+1<<"_content_check";
        bool content_check = get_config_bool(variables, ss.str(), true);
        synchropaths.push_back(new SynchroPath(source, dest, content_check));
	}
	log_enabled = get_config_bool(variables, "log_enabled", true);
	history_enabled = get_config_bool(variables, "history_enabled", true);
    external_viewer = get_config_string(variables, "external_viewer", "");
    //sprz¹tanie
    for(unsigned int i=0; i<variables->size(); i++){
        delete variables->at(i);
    }
    variables->clear();
}

void Config::save_config(){
	IO::geti()->log("Zapisywanie ustawieñ...");
    if(!file_exists(config_filename)){
        IO::geti()->log("Brak pliku konfiguracyjnego - tworzenie nowego");
        clear_file(config_filename);
    }
    //lista zmiennych - uzupe³nienie
    vector<ConfigVariable*>* variables = new vector<ConfigVariable*>;
    add_variable(variables, "save_wnd_pos", save_wnd_pos);
    add_variable(variables, "wnd_pos_x", wnd_pos_x);
    add_variable(variables, "wnd_pos_y", wnd_pos_y);
    add_variable(variables, "save_wnd_size", save_wnd_size);
    add_variable(variables, "window_w", window_w);
    add_variable(variables, "window_h", window_h);
    add_variable(variables, "log_enabled", log_enabled);
    add_variable(variables, "history_enabled", history_enabled);
    add_variable(variables, "external_viewer", external_viewer);
    add_variable(variables, "synchro_paths_num", (int)synchropaths.size());
    stringstream ss;
    for(unsigned int i=0; i<synchropaths.size(); i++){
        ss_clear(ss);
		ss<<"synchro_path_"<<i+1<<"_source";
        add_variable(variables, ss.str(), synchropaths.at(i)->source);
        ss_clear(ss);
		ss<<"synchro_path_"<<i+1<<"_dest";
        add_variable(variables, ss.str(), synchropaths.at(i)->dest);
        ss_clear(ss);
		ss<<"synchro_path_"<<i+1<<"_content_check";
        add_variable(variables, ss.str(), synchropaths.at(i)->content_check);
    }
    //zapisanie zmiennych do pliku
    vector<string>* lines = get_all_lines(config_filename);
    for(unsigned int i=0; i<variables->size(); i++){
        add_variable_line(lines, variables->at(i));
    }
    ss_clear(ss);
    for(unsigned int i=0; i<lines->size(); i++){
        ss<<lines->at(i);
        if(i<lines->size()-1) ss<<endl;
    }
    delete variables;
    delete lines;
    save_file(config_filename, ss.str());
}

vector<string>* Config::get_config_lines(string filename){
    vector<string>* lines = get_all_lines(filename);
    if(lines==NULL) return NULL;
    for(int i=0; i<(int)lines->size(); i++){
        if(lines->at(i).length()==0){ //usuniêcie pustych elementów
            lines->erase(lines->begin()+i);
            i--;
            continue;
        }
        if(lines->at(i).length()>=2){ //usuniêcie komentarzy
            if(lines->at(i)[0]==';' || (lines->at(i)[0]=='/' && lines->at(i)[1]=='/')){
                lines->erase(lines->begin()+i);
                i--;
            }
        }
    }
    return lines;
}

vector<ConfigVariable*>* Config::get_config_variables(string filename){
    vector<string>* lines = get_config_lines(filename);
    if(lines==NULL) return NULL;
    vector<ConfigVariable*>* variables = new vector<ConfigVariable*>;
    //wy³uskanie nazwy i wartoœci
    for(unsigned int i=0; i<lines->size(); i++){
        //szukanie znaku =
        for(unsigned int j=1; j<lines->at(i).length(); j++){
            if(lines->at(i)[j] == '='){
                string name = trim_spaces(lines->at(i).substr(0, j));
                string value = trim_quotes(trim_spaces(lines->at(i).substr(j+1)));
                variables->push_back(new ConfigVariable(name, value));
                break;
            }
        }
    }
    lines->clear();
    return variables;
}

string Config::get_config_string(vector<ConfigVariable*>* variables, string name, string domyslny){
    if(variables==NULL) return domyslny;
    for(unsigned int i=0; i<variables->size(); i++){
        if(variables->at(i)->name == name){
            return variables->at(i)->value;
        }
    }
    IO::geti()->error("Nie znaleziono zmiennej w pliku konfiguracyjnym: "+name);
    return domyslny;
}

int Config::get_config_int(vector<ConfigVariable*>* variables, string name, int domyslny){
    string s = get_config_string(variables, name);
    if(s.length()==0) return domyslny;
    return atoi(s.c_str());
}

bool Config::get_config_bool(vector<ConfigVariable*>* variables, string name, bool domyslny){
    string s = get_config_string(variables, name);
    if(s.length()==0) return domyslny;
    if(s=="true") return true;
    if(s=="1") return true;
    return false;
}

void Config::add_variable_line(vector<string>* lines, ConfigVariable* variable){
    //szukanie istniej¹cej linii
    for(unsigned int i=0; i<lines->size(); i++){
        if(string_begins(lines->at(i), variable->name)){
            //modyfikacja ju¿ istniejacej linii
            lines->at(i) = variable->name + " = " + variable->value;
            return;
        }
    }
    //dodanie nowej linii
    lines->push_back(variable->name + " = " + variable->value);
    lines->push_back("");
}

void Config::add_variable(vector<ConfigVariable*>* variables, string name, string value){
    variables->push_back(new ConfigVariable(name, value));
}

void Config::add_variable(vector<ConfigVariable*>* variables, string name, int value){
    stringstream ss;
    ss<<value;
    variables->push_back(new ConfigVariable(name, ss.str()));
}

void Config::add_variable(vector<ConfigVariable*>* variables, string name, bool value){
    variables->push_back(new ConfigVariable(name, value?"true":"false"));
}

