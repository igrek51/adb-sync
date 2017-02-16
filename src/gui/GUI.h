//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_GUI_H
#define ADBSYNC_GUI_H

#include "MainWindow.h"
#include "../dispatcher/IEventObserver.h"

class GUI : public IEventObserver {
public:
    GUI();

    ~GUI();

    virtual void registerEvents() override;

    virtual void onEvent(Event* e) override;

private:
    MainWindow* mainwindow;
};


#endif //ADBSYNC_GUI_H
