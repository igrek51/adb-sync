#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "synchro.h"
#include "dir.h"
#include "lista.h"
#include "history.h"

#include <iostream>
#include <sstream>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    string drive;
    lista *listalista;
    int synchro_paths_num;
    string *synchro_paths_source, *synchro_paths_dest;
    int *synchro_paths_content;
    bool thread_active;
    stringstream ss;
    //program
    void info(string i);
    void set_progress(double p);
    void ss_clear(stringstream &sstream);
    void clear_file(string filename);
    void log(string l);
    void log(int l);
    void buttons_enable(bool enable);
    void synchroman_init();
    void otworz_foldery();
    void otworz_pliki();
    void pb_usun();
    void pb_odwroc();
    void wykonaj_1();
    void wykonaj_wszystko();
    //listbox
    void listbox_init();
    int listbox_count();
    void listbox_delete(int nr);
    void listbox_clear();
    void listbox_add(string s1, string s2);
    int listbox_current();
    void listbox_select(int nr);
    void show_lista();
    void listbox_clicked();
    //dir
    dir* list_dir(string d);
    //lista
    void lista_exec1(lista *e);
    void exec_task(string l);
    void lista_exec(lista **head);
    //synchro
    void compare_files_out(string file1, string file2);
    string select_drive();
    void dirlist_cmp(string head1name, string head2name, lista **listaa, bool content_check, double prog_from, double prog_to);
    //historia
    history *historia;
    int history_enabled;

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

#endif // MAINWINDOW_H
