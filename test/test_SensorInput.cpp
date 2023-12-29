#include "SensorInput.h"
#include "SensorInput.cpp"



int main( void )
{
    SensorInput sensorInput;
    for ( int i=0; i<16; i++ ) 
    {
        std::cout << i << " ";
        sensorInput.push( i < 8 );
        //std::cout << ( int ) sensor.matches( LONG, TOUCH ) << std::endl;
        if ( sensorInput == MEDIUM_TOUCH ) { std::cout << "MEDIUM_TOUCH" << std::endl; }
        if ( sensorInput == LONG_TOUCH   ) { std::cout << "LONG_TOUCH  " << std::endl; }
        if ( sensorInput == RISING_EDGE  ) { std::cout << "RISING_EDGE " << std::endl; }
        if ( sensorInput == FALLING_EDGE ) { std::cout << "FALLING_EDGE" << std::endl; }
    }

return 0;
}
