#include "Full.h"

/*
======================================================================================================================================================
    FULL            ENTRY                                       set rate to huge
                    EXIT                                        set rate to tiny
------------------------------------------------------------------------------------------------------------------------------------------------------
                    no parent                                   change brightness
                    delay over AND light < parent's             raise brightness
                    delay over AND light > parent's             lower brightness
    IDLE            light == FULL                               leave tree
    IDLE            light ==_ZERO                               leave tree
------------------------------------------------------------------------------------------------------------------------------------------------------
*/

// FULL state, turn brightess fully up or off.
Full::Full() 
    : State( FULL_ID ) 
{ }

// Set rate to highest possible for simple on/off effect.
void Full::enter( Lantern& lantern )
{ 
    lantern.light.setRate( 7 ); 
}

void Full::exit( Lantern& lantern ) 
{ 
    lantern.light.setRate( 0 ); 
}

// Change brightness according to up/down bit, or if following a parent, towards it. 
uint8_t Full::act( Lantern& lantern )
{
    if ( lantern.parent and not lantern.delay.isOver() ) 
    { 
        return 0;
    }
    lantern.light.changeBrightness(); 
    return 0;
}

// Change to IDLE state once fully lit or fully out.
uint8_t Full::getNext( Lantern& lantern )
{
    if ( lantern.parent and not lantern.delay.isOver() )
    {
        return *this;
    }
    if ( lantern.light == LIGHT_FULL or lantern.light == 0 ) 
    { 
        lantern.parent = nullptr; 
        return IDLE_ID;
    }
    return *this;
}
