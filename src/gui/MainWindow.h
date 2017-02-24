//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_MAINWINDOW_H
#define ADBSYNC_MAINWINDOW_H

#include <QMainWindow>
#include "DiffListBox.h"

using namespace std;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {

Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0);

	~MainWindow();

signals:
	void setProgress(double p);

	void uiMessage(string msg);

	void buttonsEnable(bool enable);

	void addDiff(Diff* diff);

	void updateDiffs(vector<Diff*>* diffs);

public slots:

	void setProgressSlot(double p);

	void uiMessageSlot(string msg);

	void buttonsEnableSlot(bool enable);

	void addDiffSlot(Diff* diff);

	void updateDiffsSlot(vector<Diff*>* diffs);

private slots:
	void on_pb_scan_clicked();

	void on_pb_delete_clicked();

	void on_pb_reverse_clicked();

	void on_pb_execute_clicked();

	void on_pb_execute_all_clicked();

	void on_list1_cellClicked(int row, int column);

private:
	Ui::MainWindow* ui;

	DiffListBox* listBox;
};


#endif //ADBSYNC_MAINWINDOW_H
