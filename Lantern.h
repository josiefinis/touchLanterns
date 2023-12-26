/* 
======================================================================================================================================================
                    LANTERN
======================================================================================================================================================
*/ 

#ifndef LANTERN_H
#define LANTERN_H

#include "Arduino.h"
#include "Random.h"
#include "Light.h"
#include "SensorInput.h"
#include "DelayTimer.h"
#include "State.h"
#include "Idle.h"
#include "Wake.h"
#include "Full.h"
#include "Flicker.h"
#include "Auto.h"
#include "Pause.h"

class Idle;
class Wake;
class Full;
class Flicker;
class Auto;
class Pause;
class State;

class Lantern 
{
    public:
        static Idle     IDLE;
        static Wake     WAKE;
        static Full     FULL;
        static Flicker  FLKR;
        static Auto     AUTO;
        static Pause    PAUS;

        Lantern();

        uint8_t update( uint8_t sensorValue );
        void changeStateTo( uint8_t next );
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
        DelayTimer delay;
      
        friend class State;
        friend class Idle;
        friend class Wake;
        friend class Full;
        friend class Flicker;
        friend class Auto;
        friend class Pause;

        //friend class Bridge;
};

#define NONE                        0xFF
#define NO_CHANGE                   0x00
#define START_FLICKER               0x01 
#define FLICKER                     0x02
#define PULSE                       0x03
#define SET_BRIGHTNESS_TO_ZERO      0x04
#define SET_BRIGHTNESS_TO_FULL      0x05
#define SET_BRIGHTNESS_TO_REF       0x06
#define SET_REFERENCE_TO_ZERO       0x07 
#define SET_REF_TO_BRIGHTNESS       0x08 
#define TRACK_REFERENCE             0x09
#define TRACK_PARENT                0x0A
#define MAKE_TREE                   0x0B  
#define LEAVE_TREE                  0x0C  
#define REDUCE_DELAY                0x0D      
#define SET_DELAY                   0x40      // Specify log2 delay e.g. SET_DELAY | 3
#define LOWER_BRIGHTNESS            0x80      // Specify log2 step size e.g. LOWER_BRIGHTNESS | 1
#define RAISE_BRIGHTNESS            0xC0      // Specify log2 step size e.g. RAISE_BRIGHTNESS | 5
                      
                      
                      
                      

#define FULL_BRIGHTNESS             0xFF
#define HALF_BRIGHTNESS             0x80
#define ONE_BRIGHTNESS              0x01
#define RANDOM_DELAY                0x20        // Specify log2 upperbound e.g. SET_DELAY | RANDOM_DELAY | 3

#endif
