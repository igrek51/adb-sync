//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_APP_H
#define ADBSYNC_APP_H


#include "synchronizer/Synchronizer.h"
#include "gui/GUI.h"
#include <QApplication>

class App {
public:
    App(int argc, char** argv);

    virtual ~App();

    virtual int run();

private:
    int argc;
    char** argv;

    QApplication* a;

    GUI* gui;

    Synchronizer* synchronizer;
};


#endif //ADBSYNC_APP_H
