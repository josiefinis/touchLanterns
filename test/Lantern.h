/* 
======================================================================================================================================================
                    LANTERN
======================================================================================================================================================
*/ 

#ifndef LANTERN_H
#define LANTERN_H

#include <cstdint>
#include "StateMachine.h"
#include "TransitionMatrix.h"
#include "Random.h"


class Lantern : public StateMachine {
  public:
    static const TransitionMatrix transitionMatrix;
    Lantern();

    void pushInput( bool value );
    uint8_t classifyInput( void );
    bool nextState( void );
    bool updateOutput( void );
    bool changeBrightness( void );
    void burnDown( void );

    uint8_t getBrightness( void );
    void setBrightness( uint8_t value );
    uint8_t getStepSize( void );
    void setStepSize();
    void setDelay();
    Lantern* getParent( void );
    void setParent( Lantern* pLantern );

  private:
    uint8_t inputRegister;
    uint8_t referenceBrightness;
    uint16_t brightness;
    Lantern* parent;
    uint8_t delay;
    bool isUndershoot();
    bool isOvershoot();
};


/*
========================================
        State
========================================
*/
#define OUT                         0x00
#define GO_OUT                      0x01
#define IDLE                        0x02
#define GO_IDLE                     0x03
#define INIT_DOWN                   0x04
#define INIT_UP                     0x05
#define FULL_DOWN                   0x06
#define FULL_UP                     0x07
#define FLICKER_DOWN                0x08
#define FLICKER_UP                  0x09
#define AUTO_DOWN                   0x0A
#define AUTO_UP                     0x0B
#define PAUSE_DOWN                  0x0C
#define PAUSE_UP                    0x0D
#define ROOT_FULL_DOWN              0x0E
#define ROOT_FULL_UP                0x0F
#define ROOT_FLICKER_DOWN           0x10
#define ROOT_FLICKER_UP             0x11
#define ROOT_AUTO_DOWN              0x12
#define ROOT_AUTO_UP                0x13
#define ROOT_PAUSE_DOWN             0x14
#define ROOT_PAUSE_UP               0x15
#define WAIT                        0x16
#define WAIT_FULL_DOWN              0x17
#define WAIT_FULL_UP                0x18
#define FOLLOW                      0x19
/*
========================================
        Input
========================================
*/
#define NO_INPUT                    0x00
#define RISING_EDGE                 0x01    
#define FALLING_EDGE                0x02
#define MEDIUM_TOUCH                0x03
#define LONG_TOUCH                  0x04
#define LONG_TOUCH_FALLING_EDGE     0x05
#define LONG_RELEASE                0x06
#define AT_ZERO_BRIGHTNESS          0x07
#define AT_ONE_BRIGHTNESS           0x08
#define AT_FULL_BRIGHTNESS          0x09 
#define WAITING                     0x0A 
#define DONT_CARE                   0xFF
/*
========================================
        Output
========================================
*/
#define NO_CHANGE                   0x00
#define START_FLICKER               0x01 
#define FLICKER                     0x02
#define PULSE                       0x03
#define SET_BRIGHTNESS_TO_ZERO      0x04
#define SET_BRIGHTNESS_TO_FULL      0x05
#define SET_BRIGHTNESS_TO_REF       0x06
#define SET_REFERENCE_TO_ZERO       0x07 
#define TRACK_REFERENCE             0x08
#define MAKE_TREE                   0x09  
#define REDUCE_DELAY                0x0A      
#define SET_DELAY                   0x40      // Specify delay e.g. SET_DELAY | 8
#define LOWER_BRIGHTNESS            0x80      // Specify step size e.g. LOWER_BRIGHTNESS | 1
#define RAISE_BRIGHTNESS            0xC0      // Specify step size e.g. RAISE_BRIGHTNESS | 5
                      
                      
                      
                      

#define FULL_BRIGHTNESS             0xFF
#define ONE_BRIGHTNESS              0x01

#endif
