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


// Return the brightness value from its location in the register.
inline uint8_t Light::getBrightness( void ) const 
{ 
    return reg >> 8; 
}


// Return the rate of change of brightness from its location in the register.
inline uint16_t Light::getRate( void ) const 
{ 
    return 1 << 4 + ( reg & 0x7 );
}


// Toggle between raising brightness and lowering brightness.
inline void Light::toggleSign( void ) 
{ 
    reg ^= 0x8;
}


inline void Light::setSign( bool value ) 
{ 
    reg &= ~0x8;
    reg |= value << 3;
}


inline bool Light::getSign( void ) 
{ 
    return reg >> 3 & 1;
}


inline void Light::setNegativeRate( void ) 
{ 
    reg |= 0x8;
}


inline void Light::setPositiveRate( void ) 
{ 
    reg &= ~0x8;
}


// Return true if brightness is decreasing.
inline bool Light::isDimming( void ) const 
{ 
    return reg & 0x8; 
}


// Return true if brightness is increasing.
inline bool Light::isBrightening( void ) const 
{ 
    return not ( reg & 0x8 ); 
}


// Set brightness bits in the register.
inline void Light::setBrightness( uint8_t value ) 
{ 
    this->reg &= ~0xfff0;
    this->reg |= value << 8; 
}


// Set rate of change bits in the register.
inline void Light::setRate( uint8_t value ) 
{ 
    this->reg &= ~0x7;
    this->reg |=  0x7 & value;
}


// Raise or lower brightness depending on the value of the up/down toggle bit in the register.
void Light::changeBrightness( uint8_t floor=0, uint8_t ceil=0xFF ) 
{
    if ( isBrightening() ) { raiseBrightness( ceil ); }
    else { lowerBrightness( floor ); }
}


// Raise brightness, using rate of change stored in register.
void Light::raiseBrightness( uint8_t ceil=0xFF ) 
{
    if ( ceil < getBrightness() ) { return; }
    int step = getRate();
    if ( step > ( ceil << 8 ) - ( reg & 0xfff0 ) ) { 
        reg &= ~( 0xff << 8 ); 
        reg |= ceil << 8; 
    }
    else { reg += step; }
}


// Lower brightness, using rate of change stored in register.
void Light::lowerBrightness( uint8_t floor=0 ) 
{
    if ( floor > getBrightness() ) { return; }
    int step = getRate();
    if ( step > ( reg & 0xfff0 ) - ( floor << 8 ) ) { 
        reg &= ~( 0xff << 8 ); 
        reg |= floor << 8; 
    }
    else { reg -= step; }
}


void Light::flicker( void )
{
    reg ^= reg << 5; 
    reg ^= 0xfff0 & reg >> 7;
}


void Light::pulse( void )
{
    //TODO
}
#endif
