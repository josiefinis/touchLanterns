#ifndef PAUSE_H
#define PAUSE_H

#include "State.h"
#include "Lantern.h"

class Lantern;

class Pause : public State
{
    public:
        Pause();
        void enter( Lantern& );
        void exit( Lantern& );
        uint8_t act( Lantern& );
        uint8_t getNext( Lantern& );
};
#endif
