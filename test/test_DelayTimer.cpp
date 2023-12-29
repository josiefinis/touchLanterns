
#include <cstdint>
#include <iostream>
#include <chrono>
#include "DelayTimer.h"
#include "DelayTimer.cpp"
#include "Random.cpp"






int main( void )
{
    DelayTimer delay;
    //std::cout << "Delay set at t =\t" << millis() << " ms" << std::endl;
    for ( int i=0; i<100; i++ ) 
    {
        uint16_t startTime = millis();
        uint16_t endTime;
        delay.setRandom( 3 );
        while ( millis() - startTime < 5000U )
        {
            if ( delay.isOver() )
            {
                //std::cout << "Delay over at t =\t" << millis() << " ms" << std::endl;
                endTime = millis();
                break;
            }
        }
        std::cout << "delay = " << ( endTime - startTime ) << " ms";
        std::cout << "\t\t\t t= " << millis() << " ms, t0 = " << startTime << " ms, t1 = " << endTime << std::endl;

        //std::cout << "Timed out at t =\t" << millis() << " ms" << std::endl;
    }

    return 0;
}
