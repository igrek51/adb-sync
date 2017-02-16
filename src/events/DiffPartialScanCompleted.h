//
// Created by igrek on 16/02/17.
//

#ifndef ADBSYNC_DIFFSCANCOMPLETED_H
#define ADBSYNC_DIFFSCANCOMPLETED_H

#include "../dispatcher/Event.h"
#include "../diffs/Diff.h"

class DiffPartialScanCompleted : public Event {
public:
    Diff* newDiff;

    DiffPartialScanCompleted(Diff* newDiff) {
        this->newDiff = newDiff;
    }
};

#endif //ADBSYNC_DIFFSCANCOMPLETED_H
