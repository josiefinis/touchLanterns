/*
======================================================================================================================================================
                    SENSOR_INPUT
======================================================================================================================================================
*/

#include "SensorInput.h"


SensorInput::SensorInput() { reg = 0; }

bool SensorInput::operator == ( const uint16_t& other ) const
{
    return ( reg & other >> 8 ) == ( other & other >> 8 );
}

bool SensorInput::operator != ( const uint16_t& other ) const { return !( *this == other ); }

void SensorInput::push( bool value ) 
{
    reg <<= 1;
    reg |= value;
}
