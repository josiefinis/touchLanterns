// Lantern class. 
#ifndef LANTERN_H
#define LANTERN_H

#include "Arduino.h"
#include "Random.h"
#include "Graph.h"

#define OUT                 0x00
#define IDLE                0x10
#define GO_OUT              0x01
#define GO_IDLE             0x11
#define INIT_DOWN           0x02
#define INIT_UP             0x12
#define FULL_DOWN           0x03
#define FULL_UP             0x13
#define AUTO_DOWN           0x04
#define AUTO_UP             0x14
#define PAUSE_DOWN          0x05
#define PAUSE_UP            0x15
#define FLICKER_DOWN        0x06
#define FLICKER_UP          0x16
#define ROOT_FULL_DOWN      0x23
#define ROOT_FULL_UP        0x33
#define ROOT_AUTO_DOWN      0x24
#define ROOT_AUTO_UP        0x34
#define ROOT_PAUSE_DOWN     0x25
#define ROOT_PAUSE_UP       0x35
#define ROOT_FLICKER_DOWN   0x26
#define ROOT_FLICKER_UP     0x36
#define WAIT                0x40
#define FOLLOW              0x80
#define WAIT_FULL_DOWN      0x43
#define WAIT_FULL_UP        0x53

#define NO_CHANGE         0x0000
#define PULSE_START       0x4FFF
#define PULSE             0x4000
#define PULSE_END         0x5000
#define FLICKER           0x6000
#define UP_STATE            0x10
#define DOWN_STATE          0x00
#define BRIGHTNESS_MAX      0xFF
#define BRIGHTNESS_MIN      0x01
#define BRIGHTNESS_VAR      0x20
#define LONG                0xFF
#define MEDIUM              0x0F
#define EDGE                0x03
#define TOUCHED             0xFF
#define RELEASED            0x00
#define RISING              0x01
#define FALLING             0xFE

class Lantern {
  public:
    Lantern();
    bool changeState( void );
    bool changeOutput( void );
    void burnDown( void );
    void pushInput( uint8_t value );
    uint8_t getInput( void );

    void setIndex( uint8_t value );
    uint8_t getIndex( void );
    void setState( uint8_t value );
    uint8_t getState( void );

    uint8_t getBrightness( void );
    uint8_t getReferenceBrightness( void );
    void setDelay( uint8_t value );
    uint8_t getDelay( void );
    void setParent( Lantern* pLantern );
    Lantern* getParent( void );

  private:
    uint8_t index;
    uint8_t state;
    uint16_t input;                                             // TODO change to uint8_t and check nothing breaks.
    uint16_t output;
    uint8_t referenceBrightness;
    uint8_t delay;
    Lantern* parent;

    bool isInput( uint8_t value, uint8_t mask=0x01 ); 
    void raiseBrightness( uint8_t rate=2, uint8_t ceiling=BRIGHTNESS_MAX );
    void lowerBrightness( uint8_t rate=2, uint8_t floor=BRIGHTNESS_MIN );
    void setBrightness( uint8_t value );
    void setRate( uint8_t value );
    uint8_t getRate( void );
    Lantern* getLantern( uint8_t index );
    void makeTree();

    bool out( void );
    bool idle( void );
    bool goOut( void );
    bool goIdle( void );
    bool initUp( void );
    bool initDown( void );
    bool fullUp( void );
    bool fullDown( void );
    bool autoUp( void );
    bool autoDown( void );
    bool pauseUp( void );
    bool pauseDown( void );
    bool flickerUp( void );
    bool flickerDown( void );
    bool rootInitUp( void );
    bool rootInitDown( void );
    bool rootFullUp( void );
    bool rootFullDown( void );
    bool rootAutoUp( void );
    bool rootAutoDown( void );
    bool rootPauseUp( void );
    bool rootPauseDown( void );
    bool rootFlickerUp( void );
    bool rootFlickerDown( void );
    bool wait( void );
    bool waitFullUp( void );
    bool waitFullDown( void );
    bool follow( void );

    void printNeighbours( void );
};
#endif
