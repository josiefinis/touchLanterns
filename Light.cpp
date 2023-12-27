/*
======================================================================================================================================================
                LIGHT
======================================================================================================================================================
*/
#include "Light.h"

Light::Light( uint8_t& reference)
    : reg( 0 )
    , reference( reference )
{ }


Light::operator int() const { return getBrightness(); }


// Return the brightness value from its location in the register.
uint8_t Light::getBrightness( void ) const 
{ 
    return reg >> 4; 
}


// Set brightness bits in the register.
void Light::setBrightness( uint8_t value ) 
{ 
    reg &= ~0x0fff;
    reg |= value << 4; 
}


// Return the rate of change of brightness from its location in the register.
uint16_t Light::getBehaviour( void ) const 
{ 
    return reg >> 12 & 7;
}


// Set behaviour e.g. to flicker, small step, etc.
void Light::setBehaviour( uint8_t value ) 
{ 
    reg &= ~0x7000;
    reg |= ( value & 7 ) << 12;
}


// Toggle between raising brightness and lowering brightness.
void Light::toggleSign( void ) 
{ 
    reg ^= 0x8000;
}


void Light::setSign( bool value ) 
{ 
    reg &= ~0x8000;
    reg |= value << 0xf;
}


bool Light::getSign( void ) 
{ 
    return reg >> 0xf & 1;
}


void Light::setToDim( void ) 
{ 
    reg |= 0x8000;
}


void Light::setToBrighten( void ) 
{ 
    reg &= ~0x8000;
}


// Return true if brightness is decreasing.
bool Light::isDimming( void ) const 
{ 
    return reg & 0x8000; 
}


// Return true if brightness is increasing.
bool Light::isBrightening( void ) const 
{ 
    return not ( reg & 0x8000 ); 
}


// Raise or lower brightness depending on the value of the up/down toggle bit in the register.
void Light::changeBrightness( uint8_t floor=0, uint8_t ceil=0xFF ) 
{
    switch ( getBehaviour() )
    {
        case STABLE:
            return;

        case TINY_STEP:
            step( 1, floor, ceil );
            return;

        case SMALL_STEP:
            step( 2, floor, ceil );
            return;
        
        case LARGE_STEP:
            step( 32, floor, ceil );
            return;

        case HUGE_STEP:
            step( 256, floor, ceil );
            return;

        case FLICKER:
            flicker();
            return;

        case PULSE:
            pulse();
            return;
    }
}


// Change brightness by step size.
void Light::step( uint16_t stepsize, uint8_t floor=0, uint8_t ceil=0xFF ) 
{
    int16_t temp = reg & 0x0fff;
    if ( isDimming() ) { temp -= stepsize; }
    else                { temp += stepsize; }
    if ( isBrightening() and temp > ceil << 4 )  { setBrightness( ceil );  return; }
    if  ( isDimming() and temp < floor << 4)    { setBrightness( floor ); return; }
    reg &= ~0xfff;
    reg |= temp;
}


void Light::flicker( void )
{
    uint16_t temp = reg & 0x0fff;
    temp ^= temp << 7; 
    temp ^= temp >> 2;
    reg = temp & 0x0fff;
}


void Light::pulse( void )
{
    switch ( getBrightness() )
    {
        case 0:
            setBrightness( reference ); 
            setBehaviour( STABLE ); 
            return;

        case LIGHT_FULL:
            setBrightness( 0 ); 
            return;

        default:
            setBrightness( LIGHT_FULL ); 
            return;
    }
}
