//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_MAINWINDOW_H
#define ADBSYNC_MAINWINDOW_H

#include <QMainWindow>

//TODO zmienić nazwy slotów na angielskie

using namespace std;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {

Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void resizeEvent(QResizeEvent*);

private slots:
    void on_pb_szukaj_clicked();

    void on_pb_wykonaj_clicked();

    void on_pb_usun_clicked();

    void on_list1_cellClicked(int row, int column);

    void on_pb_otworz_clicked();

    void on_pb_wykonaj1_clicked();

    void on_pb_porownaj_clicked();

    void on_pb_odwroc_clicked();

private:
    Ui::MainWindow *ui;
};


#endif //ADBSYNC_MAINWINDOW_H
