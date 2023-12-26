#ifndef FLICKER_H
#define FLICKER_H

#include "State.h"
#include "Light.h"
#include "DelayTimer.h"
#include "Lantern.h"


class Lantern;

class Flicker : public State
{
    public:
        Flicker();
        void enter( Lantern& );
        void exit( Lantern& );
        uint8_t act( Lantern& );
        uint8_t getNext( Lantern& );
};
#endif
