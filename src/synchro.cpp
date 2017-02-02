#include "synchro.h"
#include "dir.h"
#include "mainwindow.h"

#include <fstream>

string* get_drives(int &nr){
    //TODO pobieranie dysków nie na sztywno
    nr=0;
    QDir directory("/run/media/igrek");
    directory.setFilter(QDir::Files | QDir::Dirs | QDir::Hidden | QDir::NoSymLinks);
    directory.setSorting(QDir::Name);
    QFileInfoList list = directory.entryInfoList();
    if(list.size()<2) return NULL;
    string *tab = new string [list.size()-2];
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        const char *fname = fileInfo.fileName().toStdString().c_str();
        if(strcmp(fname,".")==0) continue;
        if(strcmp(fname,"..")==0) continue;
        tab[nr] = "/run/media/igrek/"+fileInfo.fileName().toStdString();
        nr++;
    }
    return tab;
}

string list_drives(){
    int drives_n=0;
    string *drives = get_drives(drives_n);
    stringstream ss;
    for(int i=0; i<drives_n; i++){
        ss<<drives[i];
        if(i<drives_n-1) ss<<", ";
    }
    return ss.str();
}

bool files_cmp(string file1, string file2){
    fstream plik;
    plik.open(file1.c_str(),fstream::in|fstream::binary);
    if(!plik.good()){
        plik.close();
        cout<<"Blad: Pierwszy plik nie istnieje"<<endl;
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
        cout<<"Blad: Drugi plik nie istnieje"<<endl;
        return false;
    }
    plik.seekg(0,plik.end);
    unsigned int fsize2 = plik.tellg();
    char *plik2 = new char [fsize2];
    plik.seekg(0,plik.beg);
    plik.read(plik2,fsize2);
    plik.close();
    if(fsize1!=fsize2){
        delete[] plik1;
        delete[] plik2;
        return false;
    }
    int wynik = memcmp(plik1,plik2,fsize1);
    delete[] plik1;
    delete[] plik2;
    if(wynik!=0){
        return false;
    }
    return true;
}

void MainWindow::compare_files_out(string file1, string file2){
    fstream plik;
    plik.open(file1.c_str(),fstream::in|fstream::binary);
    if(!plik.good()){
        plik.close();
        info("Błąd: Pierwszy plik nie istnieje");
        return;
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
        info("Błąd: Drugi plik nie istnieje");
        return;
    }
    plik.seekg(0,plik.end);
    unsigned int fsize2 = plik.tellg();
    char *plik2 = new char [fsize2];
    plik.seekg(0,plik.beg);
    plik.read(plik2,fsize2);
    plik.close();
    ss_clear(ss);
    if(fsize1!=fsize2){
        ss<<"Pliki o różnych rozmiarach,\r\n";
    }else{
        ss<<"Pliki o równych rozmiarach,\r\n";
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
            ss<<"Różnica - bajt "<<i<<" (wiersz: "<<wiersz<<", znak: "<<znak_wiersza-1<<")";
            break;
        }
    }
    if(rowne){
        if(fsize1==fsize2){
            ss<<"Brak różnicy zawartości plików";
        }else{
            ss<<"Różnica - bajt "<<minsize<<" (ostatni wiersz: "<<wiersz<<")";
        }
    }
    info(ss.str());
    delete[] plik1;
    delete[] plik2;
}

string MainWindow::select_drive(){
    int drives_n=0;
    string *drives = get_drives(drives_n);
    int selected = -1;
    if(synchro_paths_num==0){
        info("Nie wybrano synchronizowanych folderów.");
        return "";
    }
    //sprawdzenie istnienia folderów na dysku źródłowym
    for(int i=0; i<synchro_paths_num; i++){
        if(!dir_exists(synchro_paths_source[i])){
            ss_clear(ss);
            ss<<"Błąd: Brak katalogu \""<<synchro_paths_source[i]<<"\" na dysku źródłowym";
            info(ss.str());
            return "";
        }
    }
    //szukanie istnienia folderów na dysku docelowym
    for(int i=0; i<drives_n; i++){
        bool valid=true;
        for(int j=0; j<synchro_paths_num; j++){
            if(!dir_exists(drives[i]+"/"+synchro_paths_dest[j])){
                valid=false;
                break;
            }
        }
        if(valid){
            selected=i;
            break;
        }
    }
    if(selected==-1){
        ss_clear(ss);
        ss<<"Błąd: nie znaleziono odpowiedniego dysku.\r\n";
        if(drives_n==0){
            ss<<"Brak dostępnych dysków";
        }else{
            ss<<"Dostępne dyski: ";
            ss<<list_drives();
        }
        info(ss.str());
        return "";
    }else{
        ss_clear(ss);
        ss<<"Wybrany dysk: "<<drives[selected];
        info(ss.str());
        return drives[selected];
    }
}

void MainWindow::dirlist_cmp(string head1name, string head2name, lista **listaa, bool content_check, double prog_from, double prog_to){
    set_progress(prog_from);
    dir *head1 = list_dir(head1name);
    dir *head2 = list_dir(head2name);
    int num = dirlist_num(head1), numi=0;
    if(head1==NULL&&head2==NULL) return;
    dir *pom1 = head1, *szuk;
    while(pom1!=NULL){ //sprawdzanie listy pierwszej - wzorca
        szuk = dirlist_search(head2,pom1->name);
        if(pom1->size==-1){ //katalog
            if(szuk==NULL){
                lista_add(listaa,pom1->name,head1name,head2name,1);
                show_lista();
            }else{
                if(szuk->size!=-1){
                    lista_add(listaa,pom1->name,head1name,head2name,1);
                    show_lista();
                }else{
                    dirlist_cmp(head1name+"/"+pom1->name,head2name+"/"+pom1->name,listaa,content_check,(prog_to-prog_from)*numi/num+prog_from,(prog_to-prog_from)*(numi+1)/num+prog_from);
                }
            }
        }else{ //plik
            if(szuk==NULL){
                lista_add(listaa,pom1->name,head1name,head2name,2);
                show_lista();
            }else{
                if(szuk->size!=pom1->size){
                    lista_add(listaa,pom1->name,head1name,head2name,3);
                    show_lista();
                }else{
                    if(content_check){
                        if(!files_cmp(head1name+"/"+pom1->name,head2name+"/"+pom1->name)){
                            lista_add(listaa,pom1->name,head1name,head2name,4);
                            show_lista();
                        }
                    }
                }
            }
        }
        pom1=pom1->next;
        numi++;
        set_progress((prog_to-prog_from)*numi/num+prog_from);
    }
    pom1 = head2;
    while(pom1!=NULL){ //sprawdzanie zbednych plikow z drugiej listy
        if(dirlist_search(head1,pom1->name)==NULL){ //jeśli nie znajdzie
            if(pom1->size==-1){
                lista_add(listaa,pom1->name,head1name,head2name,5);
                show_lista();
            }else{
                lista_add(listaa,pom1->name,head1name,head2name,6);
                show_lista();
            }
        }
        pom1=pom1->next;
    }
    dirlist_destroy(&head1);
    dirlist_destroy(&head2);
}
