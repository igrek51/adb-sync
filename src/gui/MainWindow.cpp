//
// Created by igrek on 04/02/17.
//

#include "MainWindow.h"
#include "../../build/.ui/ui_mainwindow.h"

#include <QDesktopWidget>

MainWindow::MainWindow(QWidget* parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QDesktopWidget widget;
    QRect mainScreenSize = widget.availableGeometry(widget.primaryScreen());
    int screen_w = mainScreenSize.width();
    int screen_h = mainScreenSize.height();
    this->move((screen_w - this->geometry().width()) / 2,
               (screen_h - this->geometry().height()) / 2);

//    set_progress(0);
//    listbox_init();
//    buttons_enable(false);

//    info("Naciśnij przycisk \"Szukaj\", aby wyszukać elementy synchronizacji.");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pb_szukaj_clicked() {

}

void MainWindow::on_pb_wykonaj_clicked() {

}

void MainWindow::on_list1_cellClicked(int, int) {

}

void MainWindow::on_pb_otworz_clicked() {

}

void MainWindow::on_pb_usun_clicked() {

}

void MainWindow::on_pb_wykonaj1_clicked() {

}

void MainWindow::on_pb_porownaj_clicked() {

}

void MainWindow::on_pb_odwroc_clicked() {

}


void MainWindow::resizeEvent(QResizeEvent*) {
    int w = this->size().width(); //560
    int h = this->size().height(); //400
//    ui->info_txt->resize(w, 60);
//    ui->progress1->resize(w - 20, 15);
//    ui->groupBox1->resize(w - 20, h - 90);
//    ui->list1->resize(w - 40, h - 175);
//
//    ui->pb_szukaj->resize((w - 40) / 4, 60);
//    ui->pb_porownaj->resize((w - 40) / 4, 30);
//    ui->pb_odwroc->resize((w - 40) / 4, 30);
//    ui->pb_wykonaj1->resize((w - 40) / 4, 30);
//    ui->pb_otworz->resize((w - 40) / 4, 30);
//    ui->pb_usun->resize((w - 40) / 4, 30);
//    ui->pb_wykonaj->resize((w - 40) / 4, 30);
//
//    ui->pb_szukaj->move(10, h - 150);
//    ui->pb_porownaj->move(10 + (w - 40) / 4, h - 150);
//    ui->pb_odwroc->move(10 + (w - 40) * 2 / 4, h - 150);
//    ui->pb_wykonaj1->move(10 + (w - 40) * 3 / 4, h - 150);
//    ui->pb_otworz->move(10 + (w - 40) / 4, h - 120);
//    ui->pb_usun->move(10 + (w - 40) * 2 / 4, h - 120);
//    ui->pb_wykonaj->move(10 + (w - 40) * 3 / 4, h - 120);
}