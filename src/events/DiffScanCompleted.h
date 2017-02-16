//
// Created by igrek on 16/02/17.
//

#ifndef ADBSYNC_DIFFSCANCOMPLETED_H
#define ADBSYNC_DIFFSCANCOMPLETED_H

#include "../dispatcher/Event.h"
#include "../diffs/Diff.h"

class DiffScanCompleted : public Event {
public:
    Diff* newDiff;

    DiffScanCompleted(Diff* newDiff) {
        this->newDiff = newDiff;
    }
};

#endif //ADBSYNC_DIFFSCANCOMPLETED_H
