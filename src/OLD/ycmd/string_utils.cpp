#include <sstream>
#include "string_utils.h"

string trim_spaces(string s) {
    //obcięcie spacji na końcu
    while (s.length() > 0 && s[s.length() - 1] == ' ') {
        s = s.substr(0, s.length() - 1);
    }
    //obcięcie spacji na początku
    while (s.length() > 0 && s[0] == ' ') {
        s = s.substr(1);
    }
    return s;
}

string trim_quotes(string s) {
    if (s.length() >= 3) {
        //jeśli cudzysłowy są na poczatku i na końcu
        if (s[0] == '\"' && s[s.length() - 1] == '\"') {
            //jeśli w całym stringu znajdują się tylko 2 cudzysłowy
            if (s.substr(1, s.length() - 2).find('\"') == string::npos) {
                s = s.substr(1, s.length() - 2); //usuń je
            }
        }
    }
    return s;
}


vector<string>* get_param_list(string lista) {
    vector<string>* kontener = new vector<string>;
    lista = trim_spaces(lista);
    if (lista.length() == 0) return kontener;
    //podział przez spacje lub przecinki (nie przez cudzysłowy)
    bool cudzyslow = false;
    string newStr;
    for (int i = 0; i < (int) lista.length(); i++) {
        if (lista[i] == '\"') cudzyslow = !cudzyslow;
        if (lista[i] == ',' && !cudzyslow) lista[i] = ' ';
        if (lista[i] == ' ' && !cudzyslow && i > 0) {
            newStr = trim_spaces(lista.substr(0, (unsigned int) i));
            if(newStr.length() > 0) {
                kontener->push_back(newStr);
            }
            lista = lista.substr((unsigned int) (i + 1));
            i = -1;
        }
    }
    newStr = trim_spaces(lista);
    if(newStr.length() > 0) kontener->push_back(newStr);
    return kontener;
}


bool ends_with(string name, string ext) {
    if (ext.length() == 0) return true;
    if (ext.length() > name.length()) return false;
    name = name.substr(name.length() - ext.length(), ext.length());
    return name == ext;
}


string replaceStr(string str, char find, char replaceTo) {
    for (unsigned int i = 0; i < str.length(); i++) {
        if (str[i] == find) str[i] = replaceTo;
    }
    return str;
}

string time_interval_string(time_t interval_s){
    long long sec = interval_s % 60;
    long long min = interval_s / 60;
    stringstream ss;
    if(min > 0){
        ss<<min<<" min ";
        if(sec<10){
            ss<<"0";
        }
    }
    ss<<sec<<" s";
    return ss.str();
}