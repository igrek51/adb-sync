#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    listalista=NULL;
    thread_active = false;

    synchro_paths_num = 2;
    synchro_paths_source = new string [synchro_paths_num];
    synchro_paths_dest = new string [synchro_paths_num];
    synchro_paths_content = new int [synchro_paths_num];

    //TODO konfiguracja z pliku config
    synchro_paths_source[0] = "/mnt/data/Igrek/mp3";
    synchro_paths_dest[0] = "Mp3";
    synchro_paths_content[0]=0;

    synchro_paths_source[1] = "/mnt/data/Igrek/guitarDB";
    synchro_paths_dest[1] = "guitarDB";
    synchro_paths_content[1]=1;

    clear_file("log.txt");
    history_enabled = 1;
    history_load(&historia);

    ui->setupUi(this);

    QDesktopWidget widget;
    QRect mainScreenSize = widget.availableGeometry(widget.primaryScreen());
    int screen_w = mainScreenSize.width();
    int screen_h = mainScreenSize.height();
    this->move((screen_w-this->geometry().width())/2,(screen_h-this->geometry().height())/2);
    set_progress(0);
    listbox_init();
    buttons_enable(false);
    info("Naciśnij przycisk \"Szukaj\", aby wyszukać elementy synchronizacji.");
}

MainWindow::~MainWindow()
{
    if(history_enabled==1){
        history_truncate(&historia,50);
        history_save(&historia);
        log("Zapisano historię poleceń");
    }
    delete ui;
}

void MainWindow::on_pb_szukaj_clicked(){
    synchroman_init();
}

void MainWindow::on_pb_wykonaj_clicked(){
    wykonaj_wszystko();
}

void MainWindow::on_list1_cellClicked(int, int){
    listbox_clicked();
}

void MainWindow::on_pb_otworz_clicked(){
    otworz_foldery();
}

void MainWindow::on_pb_usun_clicked(){
    pb_usun();
}

void MainWindow::on_pb_wykonaj1_clicked(){
    wykonaj_1();
}

void MainWindow::on_pb_porownaj_clicked(){
    otworz_pliki();
}

void MainWindow::on_pb_odwroc_clicked(){
    pb_odwroc();
}


void MainWindow::resizeEvent(QResizeEvent*){
    int w = this->size().width(); //560
    int h = this->size().height(); //400
    ui->info_txt->resize(w,60);
    ui->progress1->resize(w-20,15);
    ui->groupBox1->resize(w-20,h-90);
    ui->list1->resize(w-40,h-175);

    ui->pb_szukaj->resize((w-40)/4,60);
    ui->pb_porownaj->resize((w-40)/4,30);
    ui->pb_odwroc->resize((w-40)/4,30);
    ui->pb_wykonaj1->resize((w-40)/4,30);
    ui->pb_otworz->resize((w-40)/4,30);
    ui->pb_usun->resize((w-40)/4,30);
    ui->pb_wykonaj->resize((w-40)/4,30);

    ui->pb_szukaj->move(10,h-150);
    ui->pb_porownaj->move(10+(w-40)/4,h-150);
    ui->pb_odwroc->move(10+(w-40)*2/4,h-150);
    ui->pb_wykonaj1->move(10+(w-40)*3/4,h-150);
    ui->pb_otworz->move(10+(w-40)/4,h-120);
    ui->pb_usun->move(10+(w-40)*2/4,h-120);
    ui->pb_wykonaj->move(10+(w-40)*3/4,h-120);
}
