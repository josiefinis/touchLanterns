/* 
======================================================================================================================================================
                    LANTERN
======================================================================================================================================================
*/ 

#ifndef LANTERN_H
#define LANTERN_H

#include "Global.h"
#include "Environment.h"

#include "State.fwd.h"
#include "SerialMonitor.fwd.h"
#include "Random.h"
#include "Light.h"
#include "SensorInput.h"


class Lantern
{
    public:
        Lantern();

        void burnDown( void );
        Lantern* getParent( void );
        void setParent( Lantern* parent );
        uint8_t getBrightness( void );

    private:
        State* state;
        SensorInput input;
        Light light;
        Lantern* parent;
        uint8_t reference;
        uint8_t delay;
        friend class State;
        friend class LanternCollection;
        friend class Idle;
        friend class Wake;
        friend class Full;
        friend class Flicker;
        friend class Auto;
        friend class Pause;
        friend class Pulse;
        friend class Ripple;
        friend class Bridge;
};
#endif
