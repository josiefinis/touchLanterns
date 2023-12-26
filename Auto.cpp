#include "Auto.h"
/*
------------------------------------------------------------------------------------------------------------------------------------------------------
    AUTO                ENTER                                      set light's rate of change to 1                        

------------------------------------------------------------------------------------------------------------------------------------------------------
                        No parent                                  change brightness
                        delay over AND light > parent's            lower brightness
                        delay over AND light > parent's            raise brightness
    PAUS                at full brightness                                    
    PAUS                at 1 brightness                                       
    PAUS                light == parent's AND parent != AUTO                  
    PAUS                input == rising edge AND no parent                    
------------------------------------------------------------------------------------------------------------------------------------------------------
*/
Auto::Auto() 
    : State( AUTO_ID ) 
{ }

void Auto::enter( Lantern& lantern ) { lantern.light.setRate( 1 ); }

void Auto::exit( Lantern& lantern ) { } 

uint8_t Auto::act( Lantern& lantern )
{
    if ( not lantern.parent ) 
    {
        lantern.light.changeBrightness( 1, LIGHT_FULL );
        return 0;
    }
    if ( not lantern.delay.isOver() ) { return 0; }
    if ( lantern.light > lantern.parent->light ) 
    { 
        lantern.light.lowerBrightness( lantern.parent->light ); 
        return 0;
    }
    if ( lantern.light < lantern.parent->light )  
    { 
        lantern.light.raiseBrightness( lantern.parent->light ); 
        return 0;
    }
    return 0;
}

uint8_t Auto::getNext( Lantern& lantern )
{
    if ( lantern.light == 1 ) { return PAUS_ID; }
    if ( lantern.light == LIGHT_FULL ) { return PAUS_ID; }
    if ( lantern.input == RISING_EDGE and not lantern.parent ) { return PAUS_ID; }
    if ( lantern.light == lantern.parent->light and *( lantern.parent->state ) != AUTO_ID ) { return PAUS_ID; }
    return *this;
}
