/*
======================================================================================================================================================
                    SENSOR_INPUT
======================================================================================================================================================
*/
#include "SensorInput.h"


SensorInput::SensorInput() { state = NOT_TOUCHED; }


SensorInput::operator int() const { return ( int ) state; }


void SensorInput::push( bool value ) 
{
    switch ( value )
    {
        case 0:
            if ( state > NOT_TOUCHED ) 
            { 
                state = FALLING_EDGE; 
            }
            else 
            { 
                state = NOT_TOUCHED; 
            }
            return;

        case 1:
            if ( state >= LONG_TOUCH ) 
            { 
                state = LONG_TOUCH; 
                return;
            }
            if ( state >= RISING_EDGE )
            {
                state++;
                return;
            }
            if ( state < RISING_EDGE ) 
            {
                state = RISING_EDGE;
                return;
            }
    }
}
