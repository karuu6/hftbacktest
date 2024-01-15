#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "types.h"

class Callbacks {
public:
    virtual void trade(Trade& info) {}
};

#endif