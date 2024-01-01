/*
======================================================================================================================================================
                    SENSOR_INPUT
======================================================================================================================================================

Finite state machine that categorises input from sensor.


            ┌--------------------------------┐                                                                   ┌-----------------------┐
            1                                │                                                                   │                       │
    ┏━━━━━━━┷━━━━━━┓      ┏━━━━━━━━━━━━━━┓   ↓  ┏━━━━━━━━━━━━━━┓                  ┏━━━━━━━━━━━━━━┓               ↓  ┏━━━━━━━━━━━━━━┓     │
    ┃ FALLING_EDGE ┠-0--->┃ NOT_TOUCHED  ┠-1--->┃ RISING_EDGE  ┠-1---> ... --1--->┃ MEDIUM_TOUCH ┠-1---> ... --1--->┃ LONG_TOUCH   ┠-1---┘
    ┗━━━━━━━━━━━━━━┛   ↑  ┗━━━━━━━┯━━━━━━┛      ┗━━━━━━━┯━━━━━━┛         │        ┗━━━━━━━┯━━━━━━┛         │        ┗━━━━━━━┯━━━━━━┛     
            ↑          │          0                     0                0                0                0                0
            │          └----------┘                     │                │                │                │                │
            └-------------------------------------------┘<---------------┘<---------------┘<---------------┘<---------------┘
*/

#ifndef SENSOR_INPUT_H
#define SENSOR_INPUT_H

#include "Global.h"
#include "Environment.h"


#define FALLING_EDGE    0  
#define NOT_TOUCHED     1  
#define RISING_EDGE     2
#define MEDIUM_TOUCH    5
#define LONG_TOUCH      9


class SensorInput 
{
    public:
        operator int() const; 
        SensorInput();
        void push( bool value );

    private:
        uint8_t state;
};
#endif
