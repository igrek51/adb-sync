#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <fstream>

//TODO zmienić nazwę modułu

void MainWindow::info(string i){
	ui->info_txt->setText(i.c_str());
	//this->repaint();
	log(i);
}

void MainWindow::buttons_enable(bool enable){
    ui->pb_porownaj->setEnabled(enable);
    ui->pb_odwroc->setEnabled(enable);
    ui->pb_wykonaj1->setEnabled(enable);
    ui->pb_otworz->setEnabled(enable);
    ui->pb_usun->setEnabled(enable);
    ui->pb_wykonaj->setEnabled(enable);
}

void MainWindow::set_progress(double p){
    if(p>1) p=1;
    ui->progress1->setValue(p*100);
    this->repaint();
}

void MainWindow::ss_clear(stringstream &sstream){
    sstream.str("");
    sstream.clear();
}

void MainWindow::clear_file(string filename){
    fstream plik;
    plik.open(filename.c_str(),fstream::out|fstream::trunc);
    plik.close();
}

void MainWindow::log(string l){
    fstream plik;
    plik.open("log.txt",fstream::out|fstream::app);
    plik<<time(NULL)<<" - "<<l<<endl;
    plik.close();
}

void MainWindow::log(int l){
    ss_clear(ss);
    ss<<l;
    log(ss.str());
}

void MainWindow::synchroman_init(){
	set_progress(0);
    info("Wyszukiwanie dysku...");
	drive = select_drive();
	if(drive.length()==0) return;
	if(listalista!=NULL) lista_destroy(&listalista);
    show_lista();
	//dla każdego synchronizowanego katalogu
	for(int i=0; i<synchro_paths_num; i++){
		ss_clear(ss);
        ss<<"Przeszukiwanie folderu \""<<drive<<"/"<<synchro_paths_dest[i]<<"\"...";
        info(ss.str());
        dirlist_cmp(synchro_paths_source[i],drive+"/"+synchro_paths_dest[i],&listalista,(synchro_paths_content[i]==1)?true:false,double(i)/synchro_paths_num,double(i+1)/synchro_paths_num);
	}
	set_progress(1);
	ss_clear(ss);
	ss<<"Zakończono przeszukiwanie.\r\n";
	if(listalista==NULL){
        buttons_enable(false);
		ss<<"Brak elementów do synchronizacji.";
	}else{
        buttons_enable(true);
		ss<<"Znalezione elementy synchronizacji: "<<lista_num(listalista);
	}
    info(ss.str());
    show_lista();
}

void MainWindow::otworz_foldery(){
	int n = listbox_current();
	if(n==-1){
        info("Błąd: Nie wybrano elementu.");
		return;
	}
	if(listalista==NULL) return;
	lista *pom = listalista;
	int nr=0;
	while(pom!=NULL){
		if(nr==n){
            ss_clear(ss);
            ss<<"thunar \""<<pom->dir1.c_str()<<"\" &";
            if(system(ss.str().c_str())!=0) info("Błąd wykonywania polecenia.");
            ss_clear(ss);
            ss<<"thunar \""<<pom->dir2.c_str()<<"\" &";
            if(system(ss.str().c_str())!=0) info("Błąd wykonywania polecenia.");
			return;
		}
		nr++;
		pom=pom->next;
	}
}

void MainWindow::otworz_pliki(){
	int n = listbox_current();
	if(n==-1){
        info("Błąd: Nie wybrano elementu.");
		return;
	}
	if(listalista==NULL) return;
	lista *pom = listalista;
	int nr=0;
	while(pom!=NULL){
		if(nr==n){
			if(pom->task_code!=3&&pom->task_code!=4){
                info("Funkcja niedostępna dla tych elementów.");
				return;
			}
            ss_clear(ss);
            ss<<"gedit \""<<pom->dir1<<"/"<<pom->filename<<"\" &";
            log(ss.str());
            if(system(ss.str().c_str())!=0) info("Błąd wykonywania polecenia.");
            ss_clear(ss);
            ss<<"gedit \""<<pom->dir2<<"/"<<pom->filename<<"\" &";
            log(ss.str());
            if(system(ss.str().c_str())!=0) info("Błąd wykonywania polecenia.");
            compare_files_out(pom->dir1+"/"+pom->filename,pom->dir2+"/"+pom->filename);
            //Sleep(300);
            //SetForegroundWindow(hwnd);
			return;
		}
		nr++;
		pom=pom->next;
	}
}

void MainWindow::pb_usun(){
	if(thread_active){
        info("Błąd: Trwa przeszukiwanie.");
		return;
	}
	int cr=listbox_current();
	if(cr==-1){
        info("Błąd: Nie wybrano elementu.");
		return;
	}
	if(listalista==NULL) return;
	lista *pom = listalista;
	if(cr==0){
		listalista=listalista->next;
		delete pom;
	}else{
		int nr=1;
		while(pom->next!=NULL){
			if(nr==cr){
				lista *pom2 = pom->next;
				pom->next=pom->next->next;
				delete pom2;
				break;
			}
			nr++;
			pom=pom->next;
		}
	}
    info("Usunięto element synchronizacji z listy.");
	show_lista();
}

void MainWindow::pb_odwroc(){
	int n = listbox_current();
	if(n==-1){
        info("Błąd: Nie wybrano elementu.");
		return;
	}
	if(listalista==NULL) return;
	lista *pom = listalista;
	int nr=0;
	while(pom!=NULL){
		if(nr==n){
			lista_invert(pom);
            info("Odwrócono kierunek synchronizacji dla wybranego elementu.");
			show_lista();
			listbox_select(n);
			return;
		}
		nr++;
		pom=pom->next;
	}
}

void MainWindow::wykonaj_1(){
	if(thread_active){
        info("Błąd: Trwa przeszukiwanie.");
		return;
	}
	int cr=listbox_current();
	if(cr==-1){
        info("Błąd: Nie wybrano elementu.");
		return;
	}
	if(listalista==NULL) return;
	lista *pom = listalista;
	if(cr==0){
		lista_exec1(pom);
		listalista=listalista->next;
		delete pom;
	}else{
		int nr=1;
		while(pom->next!=NULL){
			if(nr==cr){
				lista_exec1(pom->next);
				lista *pom2 = pom->next;
				pom->next=pom->next->next;
				delete pom2;
				break;
			}
			nr++;
			pom=pom->next;
		}
	}
    info("Wykonano zadania 1 elementu synchronizacji.");
	show_lista();
	listbox_select(cr);
}

void MainWindow::wykonaj_wszystko(){
	if(thread_active){
        info("Błąd: Trwa przeszukiwanie.");
		return;
	}
    info("Wykonywanie operacji...");
	if(listalista==NULL){
        info("Lista zadań jest pusta.");
		return;
	}
	lista_exec(&listalista);
	lista_destroy(&listalista);
	show_lista();
    info("Zakończono synchronizację.");
}
