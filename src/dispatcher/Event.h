//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_EVENT_H
#define ADBSYNC_EVENT_H

#include <string>

using namespace std;

class Event {

public:
    virtual ~Event();

    virtual string className() = 0;


};


#endif //ADBSYNC_EVENT_H
