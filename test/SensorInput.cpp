/*
======================================================================================================================================================
                    SENSOR_INPUT
======================================================================================================================================================
*/

#include "SensorInput.h"


SensorInput::SensorInput() { reg = NOT_TOUCHED; }


SensorInput::operator int() const { return ( int ) reg; }


void SensorInput::push( bool value ) 
{
    switch ( value )
    {
        case 0:
            if ( reg > NOT_TOUCHED ) 
            { 
                reg = FALLING_EDGE; 
            }
            else 
            { 
                reg = NOT_TOUCHED; 
            }
            return;

        case 1:
            if ( reg >= LONG_TOUCH ) 
            { 
                reg = LONG_TOUCH; 
                return;
            }
            if ( reg >= RISING_EDGE )
            {
                reg++;
                return;
            }
            else 
            {
                reg = RISING_EDGE;
                return;
            }
    }
}
