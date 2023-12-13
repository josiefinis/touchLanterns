// Lantern class. 
#ifndef LANTERN_H
#define LANTERN_H

#include <cstdint>

#define BRIGHTNESS_MAX      0xFF
#define BRIGHTNESS_MIN      0x01
#define BRIGHTNESS_VAR      0x20

class Lantern : public StateMachine {
  public:
    static const TransitionMatrix transitionMatrix;
    Lantern();

    void burnDown( void );
    void raiseBrightness( uint8_t rate=2, uint8_t ceiling=BRIGHTNESS_MAX );
    void lowerBrightness( uint8_t rate=2, uint8_t floor=BRIGHTNESS_MIN );
    uint8_t getBrightness( void );
    void setBrightness( uint8_t value );
    uint8_t getReferenceBrightness( void );
    void setReferenceBrightness( uint8_t );
    Lantern* getParent( void );
    void setParent( Lantern* pLantern );

  private:
    uint8_t referenceBrightness;
    Lantern* parent;
};
#endif
