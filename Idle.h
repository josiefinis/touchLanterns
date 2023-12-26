#ifndef IDLE_H
#define IDLE_H

#include "State.h"
#include "Lantern.h"
#include "Light.h"
#include "DelayTimer.h"

class Lantern;

class Idle : public State
{
    public:
        Idle();
        void enter( Lantern& );
        void exit( Lantern& );
        uint8_t act( Lantern& );
        uint8_t getNext( Lantern& );
};
#endif
