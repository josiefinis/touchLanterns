#include "Flicker.h"
/*
------------------------------------------------------------------------------------------------------------------------------------------------------
    FLKR                ENTER                                    flicker light, set ref to light
                        EXIT                                     set light to ref                     
------------------------------------------------------------------------------------------------------------------------------------------------------
                        always                                   flicker                              
    FULL                input == long touch                      make tree                            
    AUTO                input == falling edge                     
------------------------------------------------------------------------------------------------------------------------------------------------------
*/
// FLKR state is a branching state 
Flicker::Flicker() : State( FLKR_ID ) { }

void Flicker::enter( Lantern& lantern )
{
    lantern.light.flicker();
    lantern.reference = lantern.light;
}

void Flicker::exit( Lantern& lantern ) 
{ 
    lantern.light.setBrightness( lantern.reference ); 
    lantern.light.setRate( 0 );
}

uint8_t Flicker::act( Lantern& lantern )
{ 
    if ( lantern.input == LONG_TOUCH )
    {
        return MAKE_TREE;
    }
    lantern.light.flicker(); 
    return 0;
}

uint8_t Flicker::getNext( Lantern& lantern )
{
    if ( lantern.input == LONG_TOUCH )
    {
        return FULL_ID;
    }
    if ( lantern.input == FALLING_EDGE ) {
        return AUTO_ID;
    }
    return *this;
}
