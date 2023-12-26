/*
======================================================================================================================================================
                    STATE
======================================================================================================================================================
*/
#ifndef STATE_H
#define STATE_H

#include "Arduino.h"
#include "Light.h"
#include "DelayTimer.h"



#define IDLE_ID     0
#define WAKE_ID     1
#define FULL_ID     2
#define FLKR_ID     3
#define AUTO_ID     4
#define PAUS_ID     5

class Lantern;

class State
{
    public:
        State ( const uint8_t id );

        operator int() const;
        virtual void enter( Lantern& );
        virtual void exit( Lantern& );
        virtual uint8_t act( Lantern& );
        virtual uint8_t getNext( Lantern& );

    private:
        const uint8_t id;
};
#endif
