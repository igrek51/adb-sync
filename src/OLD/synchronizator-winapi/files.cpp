#include "files.h"
#include "io.h"
#include "strings.h"
#include <fstream>
#include <windows.h>

File::File(string name, int size){
    this->name = name;
    this->size = size;
}

void clear_file(string filename){
	fstream plik;
	plik.open(filename.c_str(), fstream::out|fstream::trunc);
	plik.close();
}

bool file_exists(string name){
	ifstream plik(name.c_str());
	if(plik.good()){
		plik.close();
		return true;
	}else{
		plik.close();
		return false;
	}
}

bool dir_exists(string name){
    DWORD ftyp = GetFileAttributesA(name.c_str());
    if(ftyp==INVALID_FILE_ATTRIBUTES) return false;
    if(ftyp&FILE_ATTRIBUTE_DIRECTORY) return true;
    return false;
}

bool files_cmp(string file1, string file2, bool out){
	fstream plik;
	plik.open(file1.c_str(),fstream::in|fstream::binary);
	if(!plik.good()){
		plik.close();
        IO::geti()->error("Pierwszy plik nie istnieje");
		return false;
	}
	plik.seekg(0,plik.end);
	unsigned int fsize1 = plik.tellg();
	char *plik1 = new char [fsize1];
	plik.seekg(0,plik.beg);
	plik.read(plik1,fsize1);
	plik.close();
	plik.open(file2.c_str(),fstream::in|fstream::binary);
	if(!plik.good()){
		plik.close();
        IO::geti()->error("Drugi plik nie istnieje");
        delete[] plik1;
		return false;
	}
	plik.seekg(0,plik.end);
	unsigned int fsize2 = plik.tellg();
	char *plik2 = new char [fsize2];
	plik.seekg(0,plik.beg);
	plik.read(plik2,fsize2);
	plik.close();
    //szczegó³y o ró¿nicach plików na output
    if(out){
        stringstream ss;
        if(fsize1==fsize2){
            ss<<"Pliki o równych rozmiarach,\r\n";
        }else{
            ss<<"Pliki o ró¿nych rozmiarach,\r\n";
        }
        int minsize=(fsize1>fsize2)?fsize2:fsize1;
        bool rowne = true;
        int wiersz = 1;
        int znak_wiersza = 0;
        for(int i=0; i<minsize; i++){
            if(plik1[i]=='\n'){
                wiersz++;
                znak_wiersza = 0;
            }else{
                znak_wiersza++;
            }
            if(plik1[i]!=plik2[i]){
                rowne=false;
                ss<<"Ró¿nica - bajt "<<i<<" (wiersz: "<<wiersz<<", znak: "<<znak_wiersza-1<<")";
                break;
            }
        }
        if(rowne){
            if(fsize1==fsize2){
                ss<<"Brak ró¿nicy zawartoœci plików";
            }else{
                ss<<"Ró¿nica - bajt "<<minsize<<" (ostatni wiersz: "<<wiersz<<")";
            }
        }
        IO::geti()->echo(ss.str());
    }
    if(fsize1!=fsize2){
        delete[] plik1;
		delete[] plik2;
		return false;
	}
	int wynik = memcmp(plik1,plik2,fsize1);
	delete[] plik1;
	delete[] plik2;
	return wynik==0;
}

vector<string>* get_all_lines(string filename){
    vector<string>* lines = new vector<string>;
    fstream plik;
    plik.open(filename.c_str(),fstream::in|fstream::binary);
    if(!plik.good()){
        plik.close();
        return lines;
    }
    string linia;
    do{
        getline(plik,linia,'\n');
        //usuniêcie znaków \r
        for(unsigned int i=0; i<linia.length(); i++){
            if(linia[i]=='\r'){
                linia.erase(linia.begin()+i);
                i--;
            }
        }
        lines->push_back(linia);
    }while(!plik.eof());
    plik.close();
    return lines;
}

vector<string>* get_nonempty_lines(string filename){
    vector<string>* lines = get_all_lines(filename);
    if(lines==NULL) return NULL;
    for(unsigned int i=0; i<lines->size(); i++){
        if(lines->at(i).length()==0){ //usuniêcie pustych elementów
            lines->erase(lines->begin()+i);
            i--;
        }
    }
    return lines;
}


char* open_file(string filename, int &file_size){
    if(!file_exists(filename)){
		IO::geti()->error("plik \""+filename+"\" nie istnieje");
		return NULL;
	}
	fstream plik;
	plik.open(filename.c_str(), fstream::in|fstream::binary);
	if(!plik.good()){
		IO::geti()->error("B³¹d otwarcia pliku");
		plik.close();
		return NULL;
	}
	plik.seekg(0, plik.end);
	file_size = plik.tellg();
	char *file_content = new char [file_size+1];
	file_content[file_size] = 0;
	plik.seekg(0, plik.beg);
	plik.read(file_content, file_size);
	plik.close();
    return file_content;
}

bool save_file(string filename, string content){
    fstream plik;
	plik.open(filename.c_str(), fstream::out|fstream::binary);
	if(!plik.good()){
		IO::geti()->error("B³¹d œcie¿ki pliku");
		plik.close();
		return false;
	}
	plik.write(content.c_str(), content.length());
	plik.close();
    return true;
}

File* file_search(vector<File*>* files, string name){
    for(unsigned int i=0; i<files->size(); i++){
        if(string_compare_lower(files->at(i)->name, name)){
            return files->at(i);
        }
    }
    return NULL;
}

vector<File*>* list_dir(string dir){
    if(dir.length()==0) dir = ".";
    if(!dir_exists(dir)){
        IO::geti()->error("brak folderu: "+dir);
        return NULL;
    }
    WIN32_FIND_DATAA ffd;
    HANDLE hFind = FindFirstFileA((dir+"\\*").c_str(), &ffd);
    if(hFind==INVALID_HANDLE_VALUE){
        IO::geti()->error("blad otwierania folderu "+dir);
        return NULL;
    }
    vector<File*>* files = new vector<File*>;
    do{
        const char *stemp = string(ffd.cFileName).c_str();
        if(strcmp(stemp,".")==0) continue;
        if(strcmp(stemp,"..")==0) continue;
        if(strcmp(stemp,"desktop.ini")==0) continue;
        if(ffd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){
			//katalog
            files->push_back(new File(ffd.cFileName, -1));
		}else{
			//plik
            files->push_back(new File(ffd.cFileName, ffd.nFileSizeLow));
		}
    }while(FindNextFileA(hFind, &ffd)!=0);
    FindClose(hFind);
    return files;
}

vector<string>* get_drives(){
    vector<string>* drives = new vector<string>;
	int max_buffer = 256;
	char *drives_text = new char [max_buffer];
	int result = GetLogicalDriveStrings(max_buffer, drives_text);
	if(result<=0 || result>max_buffer){
		IO::geti()->error("blad odczytywania dostepnych dyskow");
		return drives;
	}
	char* single_drive = drives_text;
	while(single_drive < drives_text+result){
        drives->push_back(single_drive);
		single_drive += strlen(single_drive)+1;
	}
	delete[] drives_text;
	return drives;
}
