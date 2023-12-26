/* 
======================================================================================================================================================
                    LANTERN
======================================================================================================================================================
*/ 
#include "Lantern.h"
#include "Light.h"

// TODO Make flicker look better.
// TODO Make different variations of flicker or other effects.
// TODO handle WAKE_DOWN when low brightness


Idle    Lantern::IDLE;
Wake    Lantern::WAKE;
Full    Lantern::FULL;
Flicker Lantern::FLKR;
Auto    Lantern::AUTO;
Pause   Lantern::PAUS;


Lantern::Lantern() 
    : state( &IDLE )
    , parent( nullptr ) 
    , reference( 0 )
{}


uint8_t Lantern::update( uint8_t sensorValue=NONE )
{
    if ( sensorValue == NONE )
    {
        uint8_t retValue = state->act( *this );
        return retValue;
    }
    if ( delay ) { --delay; } 
    input.push( sensorValue );
    uint8_t retValue = state->act( *this );
    uint8_t next = state->getNext( *this ); //TODO remove arg
    if ( next != *state ) { 
        state->exit( *this );
        changeStateTo( next );
    }
    return retValue;
}


void Lantern::changeStateTo( uint8_t next )
{
    switch ( next )
    {
        case IDLE_ID:
            state = &IDLE;
            state->enter( *this );
            break;

        case WAKE_ID:
            state = &WAKE;
            state->enter( *this );
            break;

        case FULL_ID:
            state = &FULL;
            state->enter( *this );
            break;

        case FLKR_ID:
            state = &FLKR;
            state->enter( *this );
            break;

        case AUTO_ID:
            state = &AUTO;
            state->enter( *this );
            break;

        case PAUS_ID:
            state = &PAUS;
            state->enter( *this );
            break;
    }
}


void Lantern::burnDown( void )
{
    if ( not light ) {
        return;
    }
    int newReference = reference + 3 - Random::pull(3);
    if ( newReference < 0 ) 
    {
        newReference = 0;
    }
    if ( newReference > 255 ) 
    { 
        newReference = 255;
    }
    reference = newReference;
}


uint8_t Lantern::getBrightness( void )
{
    return light.getBrightness();
}


Lantern* Lantern::getParent( void )
{
    return parent;
}


void Lantern::setParent( Lantern* parent )
{
    this->parent = parent;
}
