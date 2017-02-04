#include "synchro.h"
#include "files.h"
#include "io.h"
#include "config.h"
#include "strings.h"
#include "app.h"

SynchroPath::SynchroPath(string source, string dest, bool content_check){
    this->source = source;
    this->dest = dest;
    this->content_check = content_check;
}

string select_drive(){
    vector<string>* drives = get_drives();
    if(drives->size()==0){
        IO::geti()->error("Brak dostêpnych dysków.");
        delete drives;
        return "";
    }
	string selected_drive = "";
	if(Config::geti()->synchropaths.size()==0){
		IO::geti()->error("Nie wybrano synchronizowanych folderów.");
        delete drives;
		return "";
	}
    stringstream ss;
	//sprawdzenie istnienia folderów na dysku Ÿród³owym
	for(unsigned int i=0; i<Config::geti()->synchropaths.size(); i++){
		if(!dir_exists(Config::geti()->synchropaths.at(i)->source)){
			ss_clear(ss);
			ss<<"Brak katalogu \""<<Config::geti()->synchropaths.at(i)->source<<"\" na dysku Ÿród³owym";
			IO::geti()->error(ss.str());
            delete drives;
			return "";
		}
	}
	//szukanie istnienia folderów na dysku docelowym
	for(unsigned int i=0; i<drives->size(); i++){
		bool valid = true;
		for(unsigned int j=0; j<Config::geti()->synchropaths.size(); j++){
			if(!dir_exists(drives->at(i) + Config::geti()->synchropaths.at(j)->dest)){
				valid = false;
				break;
			}
		}
		if(valid){
			selected_drive = drives->at(i);
			break;
		}
	}
	if(selected_drive.length()==0){
		ss_clear(ss);
		ss<<"Nie znaleziono odpowiedniego dysku.\r\n";
		ss<<"Dostêpne dyski: ";
        for(unsigned int i=0; i<drives->size(); i++){
            ss<<drives->at(i);
            if(i<drives->size()-1) ss<<", ";
        }
		IO::geti()->error(ss.str());
        delete drives;
		return "";
	}else{
		ss_clear(ss);
		ss<<"Wybrany dysk: "<<selected_drive;
		IO::geti()->log(ss.str());
        delete drives;
		return selected_drive;
	}
}

void dirlist_cmp(string head1name, string head2name, bool content_check, double prog_from, double prog_to){
	App::geti()->set_progress(prog_from);
    vector<File*>* dir1 = list_dir(head1name);
    vector<File*>* dir2 = list_dir(head2name);
	if(dir1==NULL || dir2==NULL) return;
    File *wzor, *szuk;
    for(unsigned int i=0; i<dir1->size(); i++){ //sprawdzanie listy pierwszej - wzorca
        wzor = dir1->at(i);
        szuk = file_search(dir2, wzor->name);
		if(wzor->size==-1){ //katalog
			if(szuk==NULL){
                add_task(wzor->name,head1name,head2name,TASK_BRAK_FOLDERU);
			}else{
				if(szuk->size!=-1){ //znalaz³o, lecz jest plikiem
                    add_task(wzor->name,head1name,head2name,TASK_BRAK_FOLDERU);
				}else{ //znalaz³o, jest te¿ katalogiem
                    double progress1 = (prog_to-prog_from)*i/dir1->size()+prog_from;
                    double progress2 = (prog_to-prog_from)*(i+1)/dir1->size()+prog_from;
                    //rekurencja
					dirlist_cmp(head1name+"\\"+wzor->name,head2name+"\\"+wzor->name,content_check,progress1, progress2);
				}
			}
		}else{ //plik
			if(szuk==NULL){
                add_task(wzor->name,head1name,head2name,TASK_BRAK_PLIKU);
			}else{
				if(szuk->size!=wzor->size){
                    add_task(wzor->name,head1name,head2name,TASK_INNY_ROZMIAR);
				}else{
					if(content_check){
						if(!files_cmp(head1name+"\\"+wzor->name,head2name+"\\"+wzor->name)){
                            add_task(wzor->name,head1name,head2name,TASK_INNA_WERSJA);
						}
					}
				}
			}
		}
		App::geti()->set_progress((prog_to-prog_from)*i/dir1->size()+prog_from);
    }
    for(unsigned int i=0; i<dir2->size(); i++){ //sprawdzanie zbednych plikow z drugiej listy
        wzor = dir2->at(i);
        if(file_search(dir1, wzor->name)==NULL){ //jeœli nie znajdzie
			if(wzor->size==-1){ //folder
                add_task(wzor->name,head1name,head2name,TASK_ZBEDNY_FOLDER);
			}else{ //plik
                add_task(wzor->name,head1name,head2name,TASK_ZBEDNY_PLIK);
			}
		}
    }
    for(unsigned int i=0; i<dir1->size(); i++){
        delete dir1->at(i);
    }
    delete dir1;
    for(unsigned int i=0; i<dir2->size(); i++){
        delete dir2->at(i);
    }
    delete dir2;
}
