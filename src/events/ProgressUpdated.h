//
// Created by igrek on 13/02/17.
//

#ifndef ADBSYNC_PROGRESSUPDATEDEVENT_H
#define ADBSYNC_PROGRESSUPDATEDEVENT_H

#include "../dispatcher/Event.h"

class ProgressUpdated : public Event {
public:
    double progress;

    ProgressUpdated(double progress) {
        this->progress = progress;
    }
};

#endif //ADBSYNC_PROGRESSUPDATEDEVENT_H
