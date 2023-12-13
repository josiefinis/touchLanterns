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
    uint8_t getBrightness( void );
    uint8_t getReferenceBrightness( void );
    Lantern* getParent( void );
    void setParent( Lantern* pLantern );

  private:
    uint8_t referenceBrightness;
    uint8_t brightness;
    Lantern* parent;
};
#endif
