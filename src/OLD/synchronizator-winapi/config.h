#ifndef CONFIG_H
#define CONFIG_H

#include "synchro.h"

#include <iostream>
#include <vector>

using namespace std;

class ConfigVariable {
public:
    ConfigVariable(string name, string value);
    string name;
    string value;
};

class Config {
private:
	static Config* instance;
    Config();
public:
    static Config* geti();
    void load_config();
    void save_config();
    vector<string>* get_config_lines(string filename);
    vector<ConfigVariable*>* get_config_variables(string filename);
    string get_config_string(vector<ConfigVariable*>* variables, string name, string domyslny = "");
    int get_config_int(vector<ConfigVariable*>* variables, string name, int domyslny = 0);
    bool get_config_bool(vector<ConfigVariable*>* variables, string name, bool domyslny = false);
    void add_variable_line(vector<string>* lines, ConfigVariable* variable);
    void add_variable(vector<ConfigVariable*>* variables, string name, string value);
    void add_variable(vector<ConfigVariable*>* variables, string name, int value);
    void add_variable(vector<ConfigVariable*>* variables, string name, bool value);
    //  Sta³e
    string program_name;
    string config_filename;
    string log_filename;
    string history_filename;
    string buttons_fontface;
    int buttons_fontsize;
    string version;
    string output_control;
    //  Zmienne
    int window_w;
    int window_h;
    int history_num;
    //  z pliku konfiguracyjnego
    int save_wnd_pos;
    int wnd_pos_x;
    int wnd_pos_y;
    int save_wnd_size;
    bool log_enabled;
    bool history_enabled;
    int synchro_paths_num;
    vector<SynchroPath*> synchropaths;
    string external_viewer;
};

#endif
