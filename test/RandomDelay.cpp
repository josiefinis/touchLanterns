#ifndef RANDOM_DELAY_H
#define RANDOM_DELAY_H


#include <cstdint>
#include <iostream>
#include <chrono>
#include "Random.cpp"


uint32_t millis( void )
{
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now().time_since_epoch()).count();
}


class RandomDelay
{
    public:
        RandomDelay() { }

        bool isOver( uint8_t expectedValue ) 
        { 
            return Random::urandom( expectedValue ) == 0; 
        }
};
#endif


int main( void )
{
    RandomDelay delay;
    //std::cout << "Delay set at t =\t" << millis() << " ms" << std::endl;
    for ( int i=0; i<100; i++ ) 
    {
        uint32_t startTime = millis();
        uint32_t endTime;
        uint32_t dt = 160 + millis();
        while ( millis() - startTime < 15000U )
        {
            if ( millis() > dt )
            {
                dt = millis() + 160; 
                if ( delay.isOver( 3 ) )
                {
 //                   std::cout << "Delay over at t =\t" << millis() << " ms" << std::endl;
                    endTime = millis();
                    break;
                }
            }
        }
        std::cout << "e = " << ( endTime - startTime ) << " ms";
        std::cout << "\t\t\t t= " << millis() << " ms, t0 = " << startTime << " ms, t1 = " << endTime << std::endl;

        //std::cout << "Timed out at t =\t" << millis() << " ms" << std::endl;
    }

    return 0;
}
