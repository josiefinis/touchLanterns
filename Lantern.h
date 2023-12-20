/* 
======================================================================================================================================================
                    LANTERN
======================================================================================================================================================
*/ 

#ifndef LANTERN_H
#define LANTERN_H

#include "Arduino.h"
#include "StateMachine.h"
#include "TransitionMatrix.h"
#include "Random.h"


class Lantern : public StateMachine {
  public:
    static const TransitionMatrix transitionMatrix;
    Lantern();

    void sense( bool value );
    uint8_t classifySensorInput( void );
    uint8_t classifyParentInput( void );
    uint8_t classifyBrightnessInput( void );
    uint8_t prioritiseInput( void );
    bool nextState( void );
    bool updateOutput( void );
    bool changeBrightness( void );
    void burnDown( void );

    uint8_t getReferenceBrightness( void );
    uint8_t getBrightness( void );
    void setBrightness( uint8_t value );
    uint8_t getStepSize( void );
    void setStepSize( uint8_t value );
    void setDelay();
    Lantern* getParent( void );
    void setParent( Lantern* parent );

    int getDelay();

  private:
    uint8_t sensorRegister;
    uint8_t referenceBrightness;
    uint16_t brightness;
    Lantern* parent;
    uint8_t delay;
    void lowerBrightness( void );
    void raiseBrightness( void );
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
#define GO_FULL_DOWN                0x17
#define GO_FULL_UP                  0x18
#define GO_FOLLOW                   0x19
#define FOLLOW                      0x1A
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
#define AT_ZERO_BRIGHTNESS          0x06
#define AT_ONE_BRIGHTNESS           0x07
#define AT_FULL_BRIGHTNESS          0x08 
#define AT_ZERO_DELAY               0x09 
#define PARENT_IS_FULL_DOWN         0x0A
#define PARENT_IS_FULL_UP           0x0B
#define PARENT_IS_FOLLOW            0x0C
#define MATCHED_PARENT              0x0D
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
