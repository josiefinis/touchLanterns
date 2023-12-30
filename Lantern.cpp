/* 
======================================================================================================================================================
                    LANTERN
======================================================================================================================================================
*/ 
#include "Lantern.h"

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
    if ( light ) { --reference; }
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
