/*
======================================================================================================================================================
                        DELAY TIMER
======================================================================================================================================================

Minimal 8-bit timer.
*/
#ifndef DELAY_TIMER_CPP
#define DELAY_TIMER_CPP

#include "DelayTimer.h"

DelayTimer::DelayTimer() 
{ 
    remaining == 0; 
}

DelayTimer::operator int() const { return remaining; }

DelayTimer& DelayTimer::operator++()
{
    ++remaining;
    return *this; // return new value by reference
}

DelayTimer DelayTimer::operator++(int)
{
    DelayTimer old = *this; // copy old value
    operator++();  // prefix increment
    return old;    // return old value
}


DelayTimer& DelayTimer::operator--()
{
    --remaining;
    return *this; // return new value by reference
}


DelayTimer DelayTimer::operator--(int)
{
    DelayTimer old = *this; // copy old value
    operator--();  // prefix decrement
    return old;    // return old value
}


// Set delay in 512 ms increments up to ~64 s.
void inline DelayTimer::set( uint8_t delay )
{ 
    remaining = delay;
}


// Clear delay so it always is over.
void inline DelayTimer::clear( void )
{ 
    remaining = 0; 
}

// Return true when the delay is over.
bool inline DelayTimer::isOver( void )
{ 
    return remaining == 0;
}


void DelayTimer::setRandom( uint8_t expectedValue ) 
{ 
    uint16_t delay = 0;
    for ( int i=0; i<10; i++ )
    {
        delay += Random::urandom( expectedValue );
    }
    remaining = delay / expectedValue;
}
#endif
