/*
======================================================================================================================================================
                    STATE
======================================================================================================================================================
*/

#include "Arduino.h"
#include "State.h"


State::State( const uint8_t id )
    : id( id )
{ }

State::operator int() const
{ 
    return id; 
}


void State::enter( Lantern& lantern ) 
{}


void State::exit( Lantern& lantern ) 
{}


uint8_t State::act( Lantern& lantern ) 
{ 
    return 0; 
}


uint8_t State::getNext( Lantern& lantern ) 
{
    return 0; 
} 
