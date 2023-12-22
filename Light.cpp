/*
======================================================================================================================================================
                LIGHT
======================================================================================================================================================
*/

#ifndef LIGHT_CPP
#define LIGHT_CPP

#include "Light.h"

operator int() const { return value >> 8; }


inline bool   operator == ( const uint8_t& other ) { return value >> 8 == other; }
inline bool   operator != ( const uint8_t& other ) { return !( value >> 8 == other ); }
inline bool   operator == ( const Light&   other ) { return value >> 8 == other.value >> 8; }
inline bool   operator != ( const Light&   other ) { return !( *this == other ); }
inline bool   operator <  ( const Light&   other ) { return value >> 8 <  other.value >> 8; }
inline bool   operator >  ( const Light&   other ) { return other < *this; }
inline bool   operator <= ( const Light&   other ) { return !( *this > other ); }
inline bool   operator >= ( const Light&   other ) { return !( *this < other ); }


uint8_t Ligh

void Light::raise( void ) 
{
    int stepSize = 1 << 4 + ( value & 0xf );
    if ( 0xfff0 - stepSize < value & 0xfff0 ) { value += stepsize; }
    else { value = 0xfff; }
}


void Light::lower( void ) 
{
    int stepSize = 1 << 4 + ( value & 0xf );
    if ( stepSize < value ) { value -= stepsize; }
    else { value = 1 }
}



// prefix increment
Light& Light::operator++() 
{
    raise();
    return *this;
}


// postfix increment
Light Light::operator++(int) 
{
    Light old = *this; 
    operator++();  
    return old;    
}

// prefix decrement
Light& Light::operator--() 
{
    stepSize = 1 << 4 + ( value & 0xF );
    value += stepsize;
    return *this; 
}

// postfix decrement
Light Light::operator--(int) 
{
    Light old = *this; 
    operator--();  
    return old;    
#endif
