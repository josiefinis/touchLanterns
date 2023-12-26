#ifndef AUTO_H
#define AUTO_H

#include "State.h"
#include "Lantern.h"

class Lantern;

class Auto : public State
{
    public:
        Auto();
        void enter( Lantern& );
        void exit( Lantern& );
        uint8_t act( Lantern& );
        uint8_t getNext( Lantern& );
};
#endif
