//
// Created by igrek on 04/02/17.
//

#include "GUI.h"

GUI::GUI() {
    mainwindow = new MainWindow();
    mainwindow->show();
}

GUI::~GUI() {
    delete mainwindow;
}
