/*
======================================================================================================================================================
                LIGHT
======================================================================================================================================================
*/

#ifndef LIGHT_CPP
#define LIGHT_CPP

#include "Light.h"

Light::Light()
{
    reg = 0;
}


Light::operator int() const { return getBrightness(); }


inline uint8_t Light::getBrightness( void ) const { 
// Return the brightness value from its location in the register.
    return reg >> 8; 
}


inline uint16_t Light::getRateOfChange( void ) const { 
// Return the rate of change of brightness from its location in the register.
    return 1 << 4 + ( reg & 0x7 );
}


inline void Light::toggleUpDown( void ) { 
// Toggle between raising brightness and lowering brightness.
    reg ^= 0x8;
}


inline bool Light::isDimming( void ) const { 
// Return true if brightness is decreasing.
    return reg & 0x8; 
}


inline bool Light::isBrightening( void ) const { 
// Return true if brightness is increasing.
    return not ( reg & 0x8 ); 
}


inline void Light::setBrightness( uint8_t value ) { 
// Set brightness bits in the register.
    this->reg &= ~0xfff0;
    this->reg |= value << 8; 
}


inline void Light::setRateOfChange( uint8_t value ) { 
// Set rate of change bits in the register.
    this->reg &= ~0x7;
    this->reg |=  0x7 & value;
}


void Light::changeBrightness( uint8_t floor=0, uint8_t ceil=0xFF ) {
// Raise or lower brightness depending on the value of the up/down toggle bit in the register.
    if ( isBrightening() ) { raiseBrightness( ceil ); }
    else { lowerBrightness( floor ); }
}


void Light::raiseBrightness( uint8_t ceil=0xFF ) {
// Raise brightness, using rate of change stored in register.
    if ( ceil < getBrightness() ) { return; }
    int step = getRateOfChange();
    if ( step > ( ceil << 8 ) - ( reg & 0xfff0 ) ) { 
        reg &= ~( 0xff << 8 ); 
        reg |= ceil << 8; 
    }
    else { reg += step; }
}


void Light::lowerBrightness( uint8_t floor=0 ) {
// Lower brightness, using rate of change stored in register.
    if ( floor > getBrightness() ) { return; }
    int step = getRateOfChange();
    if ( step > ( reg & 0xfff0 ) - ( floor << 8 ) ) { 
        reg &= ~( 0xff << 8 ); 
        reg |= floor << 8; 
    }
    else { reg -= step; }
}
#endif
