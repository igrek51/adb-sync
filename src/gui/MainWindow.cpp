//
// Created by igrek on 04/02/17.
//

#include "MainWindow.h"
#include "../../build/.ui/ui_mainwindow.h"
#include "../logger/Logger.h"
#include "../events/DiffScanButtonClicked.h"
#include "../dispatcher/EventDispatcher.h"
#include "../events/DiffRemovedButtonClicked.h"
#include "../events/ExecuteDiffButtonClicked.h"
#include "../events/ExecuteAllDiffsButtonClicked.h"
#include "../version.h"

MainWindow::MainWindow(QWidget* parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow) {
	ui->setupUi(this);

	listBox = new DiffListBox(ui->list1);

	setProgress(0);
	//TODO disable buttons until scan is completed
//    buttonsEnable(false);

	uiMessage("Click \"Scan\" to search for differences.");

	setWindowTitle(("ADB Sync " + string(VERSION)).c_str());
}

MainWindow::~MainWindow() {
	delete listBox;
	delete ui;
}

void MainWindow::on_pb_scan_clicked() {
	EventDispatcher::sendLater(new DiffScanButtonClicked());
}

void MainWindow::on_pb_delete_clicked() {
	EventDispatcher::sendLater(new DiffRemovedButtonClicked(listBox->selectedRowIndex()));
}

void MainWindow::on_pb_reverse_clicked() {
	Logger::info("click");

}

void MainWindow::on_pb_execute_clicked() {
	EventDispatcher::sendLater(new ExecuteDiffButtonClicked(listBox->selectedRowIndex()));
}

void MainWindow::on_pb_execute_all_clicked() {
	EventDispatcher::sendLater(new ExecuteAllDiffsButtonClicked());
}

void MainWindow::on_list1_cellClicked(int, int) {
//    Logger::info("click");
}

void MainWindow::uiMessage(string msg) {
	ui->ui_messages->setText(msg.c_str());
	Logger::info("UI: " + msg);
}

void MainWindow::buttonsEnable(bool enable) {
	ui->pb_delete->setEnabled(enable);
	ui->pb_execute->setEnabled(enable);
	ui->pb_execute_all->setEnabled(enable);
	ui->pb_reverse->setEnabled(enable);
//    ui->pb_scan->setEnabled(enable);
}

void MainWindow::setProgress(double p) {
	if (p > 1) p = 1;
	ui->progress1->setValue((int) (p * 100));
	this->repaint();
}

void MainWindow::addDiff(Diff* diff) {
	listBox->addDiff(diff);
}

void MainWindow::updateDiffs(vector<Diff*>* diffs) {
	listBox->update(diffs);
}
