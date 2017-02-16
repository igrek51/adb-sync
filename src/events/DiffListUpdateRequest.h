//
// Created by igrek on 16/02/17.
//

#ifndef ADBSYNC_DIFFLISTUPDATEREQUEST_H
#define ADBSYNC_DIFFLISTUPDATEREQUEST_H

#include "../dispatcher/Event.h"

class DiffListUpdateRequest : public Event {
public:
    vector<Diff*>* diffs;

    DiffListUpdateRequest(vector<Diff*>* diffs) {
        this->diffs = diffs;
    }
};

#endif //ADBSYNC_DIFFLISTUPDATEREQUEST_H
