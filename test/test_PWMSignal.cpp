#include "PWMSignal.h"
#include "PWMSignal.cpp"
#include <cstdint>
#include <iostream>
#include <bitset>


class Bridge
{
    public:
        void printEdgeQueue( PWMSignal& pwm )
        {
            pwm.edgeQueue.print();
        }
};


PWMSignal pwm;
Bridge bridge;



void test_changeDuty( void ) {
  std::cout << "Test changeDuty, ";
  pwm.changeDuty( 0, 200 );
  pwm.changeDuty( 4, 50 );
  pwm.changeDuty( 7, 250 );
  pwm.changeDuty( 8, 255 );
  pwm.changeDuty( 2, 0 );
  bridge.printEdgeQueue( pwm );
}


void test_getSignal( void ) {
    std::cout << "Test getSignal, ";
    pwm.startPeriod();
    uint16_t signal = pwm.getSignal();
    signal = pwm.getSignal();
    std::cout << std::endl; 
    for ( int i=3; i>=0; --i )
    {
        std::cout << std::bitset<4>{ ( uint16_t ) ( signal >> ( 4u * i ) & 0xf ) } << " ";
    }
}


void test_getTime( void ) {
    std::cout << "Test getTime, ";
}


void test_nextEdge( void ) {
    std::cout << "Test nextEdge, ";
    for ( int i=0; i<16; ++i )
    {
        uint16_t signal = pwm.getSignal();
        std::cout << std::endl; 
        std::cout << pwm.getTime() << "\t";
        for ( int i=3; i>=0; --i )
        {
            std::cout << std::bitset<4>{ ( uint16_t ) ( signal >> ( 4u * i ) & 0xf ) } << " ";
        }
        pwm.nextEdge();
        bridge.printEdgeQueue( pwm );
    }
}


void test_startPeriod( void ) {
  std::cout << "Test startPeriod, ";
}

   
void examine_queue() 
{
    for ( int b=0; b<256; ++b ) 
    {
        pwm.startPeriod();
        pwm.changeDuty( 1, b );
        std::cout << "q";
        bridge.printEdgeQueue( pwm );
        std::cout << "p";
        pwm.nextEdge();
        uint16_t time = pwm.getTime();

        // Second half of PWM cycle
        while ( time < 0xFFFF )
        {
            pwm.nextEdge();
            time = pwm.getTime();
        }
    }
}

void test_pwmCycle() 
{
    for ( int b=0; b<256; ++b ) 
    {
        pwm.startPeriod();
        std::cout << std::endl << b << "\t";
        uint16_t signal = pwm.getSignal();
        std::cout << pwm.getTime() << "\t";
        for ( int i=3; i>=0; --i )
        {
            std::cout << std::bitset<4>{ ( uint16_t ) ( signal >> ( 4u * i ) & 0xf ) } << " ";
        }
//        pwm.changeDuty( 1, b );
        pwm.changeDuty( 0, b );
        //std::cout << "q";
        //bridge.printEdgeQueue( pwm );
        //std::cout << "p";
        pwm.nextEdge();
        uint16_t time = pwm.getTime();
        std::cout << std::endl << "\t" << time << "\t";

        // Second half of PWM cycle
        while ( time < 0xFFFF )
        {
            signal = pwm.getSignal();
            for ( int i=3; i>=0; --i )
            {
                std::cout << std::bitset<4>{ ( uint16_t ) ( signal >> ( 4u * i ) & 0xf ) } << " ";
            }
            pwm.nextEdge();
            time = pwm.getTime();
            std::cout << std::endl << "\t" << time << "\t";
        }
    }
}

int main( void ) 
{
//    test_changeDuty();
//    test_getSignal();
//    test_nextEdge();
    test_pwmCycle();
 

  return 0;
}
