/* 
======================================================================================================================================================
                    LANTERN
======================================================================================================================================================
*/ 

#ifndef LANTERN_H
#define LANTERN_H

#include "Arduino.h"
#include "StateMachine.h"
#include "TransitionTable.h"
#include "Random.h"

struct Light {
  uint16_t value;
  bool operator =  ( const Light& object ) const;
  bool operator ++ ( const Light& object ) const;
  bool operator -- ( const Light& object ) const;
  bool operator == ( const Light& object ) const;
  bool operator <  ( const Light& object ) const;
  bool operator >  ( const Light& object ) const;
};


class Lantern : public StateMachine {
  public:
    static const TransitionTable transitionTable;
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
#define AUTO                        0x01
#define FLICKER                     0x02
#define FULL                        0x03
#define IDLE                        0x04
#define INIT                        0x05
#define PAUSE                       0x06
/*
========================================
        Input
========================================
*/
#define ALWAYS                      0x000
#define ENTRY                       0x001
#define EXIT                        0x002
// RESERVED                         0x003

#define LIGHT_EQ_FULL               0x004
#define LIGHT_EQ_ONE                0x005
#define LIGHT_EQ_PARENTS            0x006
#define LIGHT_EQ_ZERO               0x007
#define LIGHT_GT_PARENTS            0x008
#define LIGHT_GT_REF                0x009
#define LIGHT_LT_PARENTS            0x00a
#define LIGHT_LT_REF                0x00b

#define NO_PARENT                   0x010
#define PARENT_IS_AUTO              0x020
#define PARENT_IS_FULL              0x030
#define PARENT_NOT_AUTO             0x040

#define SENSOR_FALLING_EDGE         0x100
#define SENSOR_LONG_TOUCH           0x200
#define SENSOR_MEDIUM_TOUCH         0x300
#define SENSOR_RISING_EDGE          0x400    

#define STEP_IS_DOWN                0x080

#define TIMER_OVER                  0x800 
/*
========================================
        Output
========================================
*/
#define LIGHT_FLICKER               0x0010 
#define LIGHT_PULSE                 0x0020 
#define LIGHT_LOWER                 0x0030 
#define LIGHT_RAISE                 0x0040 

#define LIGHT_SET_TO_REF            0x0050 
#define REF_SET_TO_ZERO             0x0080 
#define REF_SET_TO_LIGHT            0x0100 
                                         
#define STEP_SET_TO_TINY            0x0200 
#define STEP_SET_TO_SMALL           0x0400 
#define STEP_SET_TO_LARGE           0x0600 
#define STEP_SET_TO_HUGE            0x0800 
                                         
#define TIMER_CLEAR                 0x1000
#define TIMER_SET_TO_SHORT          0x2000
#define TIMER_SET_TO_LONG           0x3000
#define TIMER_SET_TO_RANDOM         0x4000
                                         
#define MAKE_TREE                   0x8000  
#define LEAVE_TREE                  0x9000  
                      
                      
                      

#define FULL_BRIGHTNESS             0xFF
#define HALF_BRIGHTNESS             0x80
#define ONE_BRIGHTNESS              0x01
#define RANDOM_DELAY                0x20        // Specify log2 upperbound e.g. SET_DELAY | RANDOM_DELAY | 3

#endif
