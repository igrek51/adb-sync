//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_IEVENTOBSERVER_H
#define ADBSYNC_IEVENTOBSERVER_H

#include "Event.h"

class IEventObserver{
public:
    IEventObserver();

    ~IEventObserver();

    virtual void onEvent(Event* e) = 0;

    virtual void registerEvents() = 0;
};

#endif //ADBSYNC_IEVENTOBSERVER_H
