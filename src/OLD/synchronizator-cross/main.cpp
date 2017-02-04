#include "mainwindow.h"
#include <QApplication>

//TODO przebudować projekt, komentarze, optimize imoprts, reformat code
//TODO funkcje indywidualne dla OS przenieść do system.h
//TODO merge z wersją Winapi

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
