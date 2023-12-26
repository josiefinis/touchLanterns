#ifndef WAKE_H
#define WAKE_H

#include "State.h"
#include "Lantern.h"

class Lantern;

class Wake : public State
{
    public:
        Wake();
        void enter( Lantern& );
        void exit( Lantern& );
        uint8_t act( Lantern& );
        uint8_t getNext( Lantern& );
};
#endif
