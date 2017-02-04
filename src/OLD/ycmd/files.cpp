#include "files.h"
#include "args.h"
#include "string_utils.h"
#include "system.h"
#include "path.h"

#include <fstream>
#include <sys/stat.h>
#include <cstring>
#include <dirent.h>

bool file_exists(string filename) {
    fstream plik;
    plik.open(filename.c_str(), fstream::in | fstream::binary);
    bool good = plik.good();
    plik.close();
    return good;
}

bool dir_exists(string name) {
    struct stat sb;
    if (stat(name.c_str(), &sb) == 0) {
        if (S_ISDIR(sb.st_mode)) {
            return true;
        }
    }
    return false;
}


bool files_equal(string file1, string file2) {
    if (!file_exists(file1)) return false;
    if (!file_exists(file2)) return false;
    fstream plik1, plik2;
    //rozmiary plików
    plik1.open(file1.c_str(), fstream::in | fstream::binary);
    plik1.seekg(0, plik1.end);
    unsigned int fsize1 = (unsigned int) plik1.tellg();
    plik2.open(file2.c_str(), fstream::in | fstream::binary);
    plik2.seekg(0, plik2.end);
    unsigned int fsize2 = (unsigned int) plik2.tellg();
    if (fsize1 != fsize2) { //różne rozmiary
        plik1.close();
        plik2.close();
        return false;
    }
    //zawartość plików
    char* plik1_tab = new char[fsize1];
    plik1.seekg(0, plik1.beg);
    plik1.read(plik1_tab, fsize1);
    plik1.close();
    char* plik2_tab = new char[fsize2];
    plik2.seekg(0, plik2.beg);
    plik2.read(plik2_tab, fsize2);
    plik2.close();
    if (memcmp(plik1_tab, plik2_tab, fsize1) != 0) { //różna zawartość
        delete[] plik1_tab;
        delete[] plik2_tab;
        return false;
    }
    delete[] plik1_tab;
    delete[] plik2_tab;
    return true;
}

bool copy_overwrite_file(string src, string file2) {
    ifstream source(src.c_str(), fstream::binary);
    if (!source.good()) return false;
    ofstream dest(file2.c_str(), fstream::trunc | fstream::binary);
    if (!dest.good()) return false;
    dest << source.rdbuf();
    source.close();
    dest.close();
    return true;
}

bool delete_file(string filename) {
    return remove(filename.c_str()) == 0;
}


vector<string>* get_all_lines(string filename) {
    filename = Path::reformat(filename);
    fstream plik;
    plik.open(filename.c_str(), fstream::in | fstream::binary);
    if (!plik.good()) {
        plik.close();
        return NULL;
    }
    vector<string>* lines = new vector<string>;
    string linia;
    do {
        getline(plik, linia, '\n'); //rozdzielenie znakami \n
        for (unsigned int i = 0; i < linia.length(); i++) { //usunięcie znaków \r
            if (linia[i] == '\r') {
                linia.erase(linia.begin() + i);
                i--;
            }
        }
        lines->push_back(linia);
    } while (!plik.eof());
    plik.close();
    return lines;
}

vector<string>* get_nonempty_lines(string filename) {
    vector<string>* lines = get_all_lines(filename);
    if (lines == NULL) return NULL;
    for (unsigned int i = 0; i < lines->size(); i++) {
        if (lines->at(i).length() == 0) { //usunięcie pustych elementów
            lines->erase(lines->begin() + i);
            i--;
        }
    }
    return lines;
}


vector<string>* get_files_from_dir(string dirStr, string ext) {
    if (ext == "*")
        ext = "";
    if (dirStr.length() == 0)
        dirStr = ".";
    if (!dir_exists(dirStr)) {
        Log::error("brak folderu: " + dirStr);
        return NULL;
    }
    DIR* d;
    struct dirent* dir;
    d = opendir(dirStr.c_str());
    if (!d) {
        Log::error("blad otwierania folderu " + dirStr);
        return NULL;
    }
    vector<string>* files = new vector<string>;
    while ((dir = readdir(d)) != NULL) {
        char* name = dir->d_name;
        if (strcmp(name, ".") == 0) continue;
        if (strcmp(name, "..") == 0) continue;
        if (dir->d_type == DT_REG) { //plik
            if (ends_with(name, ext)) {
                files->push_back(name);
            }
        }
    }
    closedir(d);
    return files;
}

vector<string>* get_dirs_from_dir(string dirStr) {
    if (dirStr.length() == 0)
        dirStr = ".";
    if (!dir_exists(dirStr)) {
        Log::error("brak folderu: " + dirStr);
        return NULL;
    }
    DIR* d;
    struct dirent* dir;
    d = opendir(dirStr.c_str());
    if (!d) {
        Log::error("blad otwierania folderu " + dirStr);
        return NULL;
    }
    vector<string>* dirs = new vector<string>;
    while ((dir = readdir(d)) != NULL) {
        char* name = dir->d_name;
        if (strcmp(name, ".") == 0) continue;
        if (strcmp(name, "..") == 0) continue;
        if (dir->d_type == DT_DIR) { //folder
            dirs->push_back(name);
        }
    }
    closedir(d);
    return dirs;
}

vector<string>* get_files_from_dir_recursively(string dirStr, string ext) {
    if (ext == "*")
        ext = "";
    if (dirStr.length() == 0)
        dirStr = ".";
    if (!dir_exists(dirStr)) {
        Log::error("brak folderu: " + dirStr);
        return NULL;
    }
    DIR* d;
    struct dirent* dir;
    d = opendir(dirStr.c_str());
    if (!d) {
        Log::error("blad otwierania folderu " + dirStr);
        return NULL;
    }
    vector<string>* files = new vector<string>;
    while ((dir = readdir(d)) != NULL) {
        char* name = dir->d_name;
        if (strcmp(name, ".") == 0) continue;
        if (strcmp(name, "..") == 0) continue;
        if (dir->d_type == DT_REG) { //plik
            if (ends_with(name, ext)) {
                files->push_back(name);
            }
        }
        if (dir->d_type == DT_DIR) { //folder
            //rekursywne wejście wgłąb
            vector<string>* subfolderFiles = get_files_from_dir_recursively(Path::append(dirStr, name), ext);
            for(string subfile : *subfolderFiles){
                files->push_back(Path::append(name, subfile));
            }
            delete subfolderFiles;
        }
    }
    closedir(d);
    return files;
}

bool mkdir_if_n_exist(string dir) {
    if (!dir_exists(dir)) {
        if (system2("mkdir " + dir)) {
            Log::debug("Utworzono katalog: " + dir);
            return true;
        } else {
            return false;
        }
    }
    return true;
}

bool mkdir_recursively(string dir) {
    if (dir_exists(dir)) return true;
    Path* parentPath = new Path(dir);
    string parentDir = parentPath->parentDir()->getPath();
    delete parentPath;
    if (parentDir.length() == 0) {
        return true;
    }
    if (dir_exists(parentDir)) {
        return mkdir_if_n_exist(dir);
    } else {
        bool result1 = mkdir_recursively(parentDir);
        if (!result1) return false;
        return mkdir_if_n_exist(dir);
    }
}

bool mkdir_overwrite_file(string src, string file2) {
    //stworzenie katalogu, jeśli nie istnieje
    Path* parentPath = new Path(file2);
    string dir = parentPath->parentDir()->getPath();
    delete parentPath;
    if (dir.length() > 0) {
        if (!mkdir_recursively(dir)) {
            Log::error("Blad tworzenia katalogu: " + dir);
            return false;
        }
    }
    //skopiowanie z nadpisaniem
    return copy_overwrite_file(src, file2);
}