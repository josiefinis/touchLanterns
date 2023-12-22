/*
======================================================================================================================================================
                LIGHT
======================================================================================================================================================
*/

#ifndef LIGHT_CPP
#define LIGHT_CPP

#include "Light.h"

Light::operator int() const { return value >> 8; }


//inline bool   Light::operator == ( const Light&   other ) { return value >> 8 == other.value >> 8; }
//inline bool   Light::operator != ( const Light&   other ) { return !( *this == other ); }
//inline bool   Light::operator <  ( const Light&   other ) { return value >> 8 <  other.value >> 8; }
//inline bool   Light::operator >  ( const Light&   other ) { return other < *this; }
//inline bool   Light::operator <= ( const Light&   other ) { return !( *this > other ); }
//inline bool   Light::operator >= ( const Light&   other ) { return !( *this < other ); }


inline uint8_t  Light::stepSize( void ) { return value & 0xf; }


void Light::set( uint8_t value ) { this->value = value << 8; }

void Light::raise( void ) 
{
    int stepSize = 1 << 4 + ( value & 0xf );
    if ( stepSize < 0xfff0 - ( value >> 4 ) ) { value += stepSize; }
    else { value = 0xfff0; }
}


void Light::lower( void ) 
{
    int stepSize = 1 << 4 + ( value & 0xf );
    if ( stepSize < value >> 4 ) { value -= stepSize; }
    else { value = 0; }
}
#endif
