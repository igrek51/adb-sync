#include "dir.h"
#include "mainwindow.h"

bool dir_exists(string dirName_in){
    return QDir(dirName_in.c_str()).exists();
}

void dirlist_add(dir **head, string name, int size){
    dir *nowy = new dir;
    nowy->name = name;
    nowy->size = size;
    nowy->next = NULL;
    if(*head==NULL){
        *head = nowy;
    }else{
        dir *last = *head;
        while(last->next!=NULL){
            last=last->next;
        }
        last->next = nowy;
    }
}

void dirlist_destroy(dir **head){
    dir *pom = *head, *pom2;
    while(pom!=NULL){
        pom2 = pom;
        pom = pom->next;
        delete pom2;
    }
    *head = NULL;
}

int dirlist_num(dir *head){
    int nr=0;
    dir *pom1 = head;
    while(pom1!=NULL){
        nr++;
        pom1=pom1->next;
    }
    return nr;
}

char to_lower(char c){
    if(c>='A'&&c<='Z') return c+32;
    return c;
}

bool str_cmp_lc(string s1, string s2){
    if(s1.length()!=s1.length()) return false;
    for(unsigned int i=0; i<s1.length(); i++){
        if(to_lower(s1[i])!=to_lower(s2[i])) return false;
    }
    return true;
}

dir* dirlist_search(dir *head, string name){
    dir *pom1 = head;
    while(pom1!=NULL){
        if(str_cmp_lc(pom1->name,name)) return pom1;
        pom1=pom1->next;
    }
    return NULL;
}

dir* MainWindow::list_dir(string d){
    if(!dir_exists(d)){
        ss_clear(ss);
        ss<<"Błąd: Folder \""<<d<<"\" nie istnieje";
        info(ss.str());
        return NULL;
    }
    QDir directory(d.c_str());
    directory.setFilter(QDir::Files | QDir::Dirs | QDir::Hidden | QDir::NoSymLinks);
    directory.setSorting(QDir::Name);
    QFileInfoList list = directory.entryInfoList();
    dir *head = NULL;
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        const char *stemp = fileInfo.fileName().toStdString().c_str();
        if(strcmp(stemp,".")==0) continue;
        if(strcmp(stemp,"..")==0) continue;
        if(strcmp(stemp,"desktop.ini")==0) continue;
        if(fileInfo.isDir()){
            dirlist_add(&head,fileInfo.fileName().toStdString(),-1); //katalog
        }else{
            dirlist_add(&head,fileInfo.fileName().toStdString(),fileInfo.size()); //plik
        }
    }
    return head;
}
