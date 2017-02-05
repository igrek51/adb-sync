//
// Created by igrek on 04/02/17.
//

#include "App.h"
#include "logger/Logger.h"
#include <QApplication>

App::App(int argc, char** argv) {
    this->argc = argc;
    this->argv = argv;
    gui = new GUI();
    synchronizer = new Synchronizer();
}

App::~App() {

}

void App::exit() {

}

int App::run() {

    Logger::info("hello");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}



