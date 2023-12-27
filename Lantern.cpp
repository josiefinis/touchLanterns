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


Lantern::Lantern() 
    : state( nullptr )
    , parent( nullptr ) 
    , reference( 0 )
    , light( Light( reference ) )
{ }


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
