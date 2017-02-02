#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::listbox_init(){
    for(int i=0; i<2; i++) ui->list1->insertColumn(0);
    ui->list1->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

int MainWindow::listbox_count(){
    return ui->list1->rowCount();
}

void MainWindow::listbox_delete(int nr){
    ui->list1->removeRow(nr);
}

void MainWindow::listbox_clear(){
    while(ui->list1->rowCount()>0) ui->list1->removeRow(0);
}

void MainWindow::listbox_add(string s1, string s2){
    ui->list1->insertRow(ui->list1->rowCount());
    QTableWidgetItem *item = new QTableWidgetItem(s1.c_str());
    item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->list1->setItem(ui->list1->rowCount()-1,0,item);
    item = new QTableWidgetItem(s2.c_str());
    item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    ui->list1->setItem(ui->list1->rowCount()-1,1,item);
}

int MainWindow::listbox_current(){
    QList <QTableWidgetItem *> zaznaczone = ui->list1->selectedItems();
    if(zaznaczone.size()==0) return -1;
    return ui->list1->currentRow();
}

void MainWindow::listbox_select(int nr){
	if(nr>=listbox_count()) nr=-1;
    ui->list1->setCurrentCell(nr,0);
}

void MainWindow::show_lista(){
    listbox_clear();
    if(listalista==NULL) return;
    lista *pom = listalista;
    int nr=1;
    while(pom!=NULL){
        listbox_add(get_task_name(pom)+":",pom->dir2+"/"+pom->filename);
        nr++;
        pom=pom->next;
    }
}

void MainWindow::listbox_clicked(){
    info("klik");
	int n = listbox_current();
	if(n==-1) return;
	if(listalista==NULL) return;
	lista *pom = listalista;
	int nr=0;
	while(pom!=NULL){
		if(nr==n){
			ss_clear(ss);
			int task_num = get_task_num(pom);
            ss<<"Zadania dla elementu "<<pom->dir2<<"/"<<pom->filename<<" ["<<task_num<<"]:\r\n";
			string *tasks = get_tasks(pom);
			for(int i=0; i<task_num; i++){
				ss<<tasks[i];
				if(i<task_num-1) ss<<"\r\n";
			}
            info(ss.str());
			break;
		}
		nr++;
		pom=pom->next;
	}
}
