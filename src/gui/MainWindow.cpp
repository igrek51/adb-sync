//
// Created by igrek on 04/02/17.
//

#include "MainWindow.h"
#include "../../build/.ui/ui_mainwindow.h"
#include "../logger/Logger.h"
#include "../events/DiffScanButtonClicked.h"
#include "../dispatcher/EventDispatcher.h"

MainWindow::MainWindow(QWidget* parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);

    listBox = new DiffListBox(ui->list1);

//    QDesktopWidget widget;
//    QRect mainScreenSize = widget.availableGeometry(widget.primaryScreen());
//    int screen_w = mainScreenSize.width();
//    int screen_h = mainScreenSize.height();
//    this->move((screen_w - this->geometry().width()) / 2,
//               (screen_h - this->geometry().height()) / 2);

    setProgress(0);
//    listbox_init();
    buttonsEnable(false);

    uiMessage("Click \"Scan\" to search for differences.");
}

MainWindow::~MainWindow() {
    delete listBox;
    delete ui;
}

void MainWindow::on_pb_scan_clicked() {
    Logger::info("click");
    EventDispatcher::sendLater(new DiffScanButtonClicked());
}

void MainWindow::on_pb_delete_clicked() {
    Logger::info("click");

}

void MainWindow::on_pb_reverse_clicked() {
    Logger::info("click");

}

void MainWindow::on_pb_execute_clicked() {
    Logger::info("click");

}

void MainWindow::on_pb_execute_all_clicked() {
    Logger::info("click");

}

void MainWindow::on_list1_cellClicked(int, int) {
    Logger::info("click");

}


void MainWindow::resizeEvent(QResizeEvent*) {
    int w = this->size().width(); //560
    int h = this->size().height(); //400
    //TODO responsive layout
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

void MainWindow::uiMessage(string msg){
    ui->ui_messages->setText(msg.c_str());
    Logger::info("UI: " + msg);
}

void MainWindow::buttonsEnable(bool enable){
    ui->pb_delete->setEnabled(enable);
    ui->pb_execute->setEnabled(enable);
    ui->pb_execute_all->setEnabled(enable);
    ui->pb_reverse->setEnabled(enable);
//    ui->pb_scan->setEnabled(enable);
}

void MainWindow::setProgress(double p){
    if(p>1) p=1;
    ui->progress1->setValue((int) (p * 100));
    this->repaint();
}

void MainWindow::addDiff(Diff* diff) {
    listBox->addDiff(diff);
}

void MainWindow::updateDiffs(vector<Diff*>* diffs) {
    listBox->update(diffs);
}
