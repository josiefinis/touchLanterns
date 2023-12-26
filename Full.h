#ifndef FULL_H
#define FULL_H

#include "State.h"
#include "Lantern.h"

class Lantern;

class Full : public State
{
    public:
        Full();
        void enter( Lantern& );
        void exit( Lantern& );
        uint8_t act( Lantern& );
        uint8_t getNext( Lantern& );
};
#endif
