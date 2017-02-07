//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_APP_H
#define ADBSYNC_APP_H


#include "synchronizer/Synchronizer.h"
#include "gui/GUI.h"

class App {
public:
    App(int argc, char** argv);

    ~App();

    int run();

private:
    int argc;
    char** argv;

    GUI* gui;

    Synchronizer* synchronizer;

};


#endif //ADBSYNC_APP_H
