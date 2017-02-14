#include "App.h"

//#include <QLabel>
//#include <QMainWindow>
//#include <QApplication>

//TODO smart pointers, shared_ptr ...
//TODO c++ 11 standards: nullptr, foreach, auto, enum class

int main(int argc, char** argv) {
//    return TestApp(argc, argv).run();
    return App(argc, argv).run();

//    //QApplication Test
//    QApplication application(argc, argv);
//    QMainWindow mainWindow;
//    QLabel *label = new QLabel(&mainWindow);
//    label->setText("first line\nsecond line");
//    mainWindow.show();
//    return application.exec();
}
