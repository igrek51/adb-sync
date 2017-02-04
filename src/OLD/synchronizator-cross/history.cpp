#include "history.h"
#include <fstream>
#include <time.h>
#include <sstream>

void history_add(history **head, string txt){
	history *nowy = new history;
	nowy->txt = txt;
	nowy->next = NULL;
	if(*head==NULL){
		*head = nowy;
	}else{
		history *last = *head;
		while(last->next!=NULL){
			last=last->next;
		}
		last->next = nowy;
	}
}

void history_add_time(history **head, string txt){
	time_t rawtime;
	struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  stringstream ss;
  if(timeinfo->tm_hour<10) ss<<"0";
  ss<<timeinfo->tm_hour<<":";
  if(timeinfo->tm_min<10) ss<<"0";
  ss<<timeinfo->tm_min<<":";
  if(timeinfo->tm_sec<10) ss<<"0";
  ss<<timeinfo->tm_sec<<", ";
  if(timeinfo->tm_mday<10) ss<<"0";
  ss<<timeinfo->tm_mday<<".";
  if(timeinfo->tm_mon+1<10) ss<<"0";
  ss<<timeinfo->tm_mon+1<<".";
  ss<<timeinfo->tm_year+1900<<" - "<<txt;
  history_add(head,ss.str());
}

void history_destroy(history **head){
	history *pom = *head, *pom2;
	while(pom!=NULL){
		pom2 = pom;
		pom = pom->next;
		delete pom2;
	}
	*head = NULL;
}

void history_delete1(history **head){
	if(*head==NULL) return;
	history *pom = *head;
	*head = (*head)->next;
	delete pom;
}

int history_num(history *head){
	int nr=0;
	history *pom1 = head;
	while(pom1!=NULL){
		nr++;
		pom1=pom1->next;
	}
	return nr;
}

void history_load(history **head){
	*head = NULL;
	fstream plik;
	plik.open("history.txt",fstream::in|fstream::binary);
	if(!plik.good()){
		plik.close();
		return;
	}
	string linia, linia2;
	do{
		getline(plik,linia,'\n');
		linia2="";
		for(unsigned int i=0; i<linia.length(); i++){
			if(linia[i]=='\r'||linia[i]=='\n') continue;
			linia2+=linia[i];
		}
		if(linia2.length()==0) continue;
		history_add(head,linia2);
	}while(!plik.eof());
	plik.close();
}

void history_save(history **head){
	fstream plik;
	plik.open("history.txt",fstream::out|fstream::binary|fstream::trunc);
	if(!plik.good()){
		plik.close();
		return;
	}
	history *pom1 = *head;
	while(pom1!=NULL){
		plik<<pom1->txt<<"\r\n";
		pom1=pom1->next;
	}
	plik.close();
}

void history_truncate(history **head, int nr){
	int times = history_num(*head)-nr;
	for(int i=0; i<times; i++){
		history_delete1(head);
	}
}
