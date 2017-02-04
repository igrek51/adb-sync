#include "variables.h"
#include "files.h"
#include "args.h"
#include "string_utils.h"
#include "path.h"

#include <cstdlib>

Variable::Variable(string name, string value) {
    this->name = name;
    this->value = value;
}

vector<string> *get_variables_lines(string filename) {
    vector<string> *lines = get_all_lines(filename);
    if (lines == NULL) return NULL;
    for (int i = 0; i < (int) lines->size(); i++) {
        if (lines->at((unsigned int) i).length() == 0) { //usunięcie pustych elementów
            lines->erase(lines->begin() + i);
            i--;
            continue;
        }
        if (lines->at((unsigned int) i).length() >= 1) { //usunięcie komentarzy #
            if (lines->at((unsigned int) i)[0] == '#') {
                lines->erase(lines->begin() + i);
                i--;
                continue;
            }
        }
        if (lines->at((unsigned int) i).length() >= 2) { //usunięcie komentarzy //
            if (lines->at((unsigned int) i)[0] == '/' && lines->at((unsigned int) i)[1] == '/') {
                lines->erase(lines->begin() + i);
                i--;
                continue;
            }
        }
    }
    return lines;
}

vector<Variable *> *get_variables(string filename) {
    vector<string> *lines = get_variables_lines(filename);
    if (lines == NULL) return NULL;
    vector<Variable *> *variables = new vector<Variable *>;
    //wyłuskanie nazwy i wartości
    for (unsigned int i = 0; i < lines->size(); i++) {
        //szukanie znaku =
        for (unsigned int j = 1; j < lines->at(i).length(); j++) {
            if (lines->at(i)[j] == '=') {
                string name = lines->at(i).substr(0, j);
                string value = lines->at(i).substr(j + 1);
                name = trim_spaces(name);
                value = trim_quotes(trim_spaces(value));
                variables->push_back(new Variable(name, value));
                break;
            }
        }
    }
    delete lines;
    return variables;
}


string get_var_string(vector<Variable *> *variables, string name, string domyslny) {
    if (variables == NULL) return domyslny;
    for (unsigned int i = 0; i < variables->size(); i++) {
        if (variables->at(i)->name == name) {
            return variables->at(i)->value;
        }
    }
    Log::error("Nie znaleziono zmiennej: " + name);
    return domyslny;
}

int get_var_int(vector<Variable *> *variables, string name, int domyslny) {
    string s = get_var_string(variables, name);
    if (s.length() == 0) return domyslny;
    return atoi(s.c_str());
}

bool get_var_bool(vector<Variable *> *variables, string name, bool domyslny) {
    string s = get_var_string(variables, name);
    if (s.length() == 0) return domyslny;
    if (s == "true") return true;
    return s == "1";
}


vector<string> *get_list_ex(string lista, string dir) {
    vector<string> *kontener = get_param_list(lista);
    for (unsigned int i = 0; i < kontener->size(); i++) {
        //obetnij plusy z początku
        if (kontener->at(i)[0] == '+') {
            kontener->at(i).erase(0, 1);
        }
        //utnij cudzyslowy po obu stronach
        kontener->at(i) = Path::reformat(trim_quotes(kontener->at(i)));
    }
    //dodawanie elementów z gwiazdką
    for (unsigned int i = 0; i < kontener->size(); i++) {
        if (kontener->at(i)[0] == '*') { //dodaj wszystko (wszystko z podanym rozszerzeniem)
            string ext = kontener->at(i).substr(1, kontener->at(i).length() - 1);
            //zastąp ten element pasującymi plikami z folderu
            kontener->erase(kontener->begin() + i);
            vector<string> *pliki = get_files_from_dir_recursively(dir, ext);
            for (unsigned j = 0; j < pliki->size(); j++) {
                add_to_set(kontener, Path::reformat(pliki->at(j)));
            }
            delete pliki;
            i--;
        }
    }
    //odejmowanie elementów
    for (int i = 0; i < (int) kontener->size(); i++) {
        if (kontener->at((unsigned int) i)[0] == '-') {
            string elem_usun = kontener->at((unsigned int) i).substr(1, kontener->at(
                    (unsigned int) i).length() - 1);
            //usuń ten element i wszystkie jego wystąpienia
            kontener->erase(kontener->begin() + i);
            remove_from_list(kontener, elem_usun);
            //szukaj od nowa
            i = -1;
        }
    }
    return kontener;
}

//TODO użyć kontenera set z STL
void add_to_set(vector<string> *kontener, string elem) {
    //jeśli już istnieje
    for (unsigned int i = 0; i < kontener->size(); i++) {
        if (kontener->at(i) == elem)
            return;
    }
    kontener->push_back(elem);
}

void remove_from_list(vector<string> *kontener, string elem) {
    //usuń wszystkie jego wystąpienia
    for (int j = 0; j < (int) kontener->size(); j++) {
        if (kontener->at((unsigned int) j) == elem) {
            kontener->erase(kontener->begin() + j);
            j--;
        }
    }
}
