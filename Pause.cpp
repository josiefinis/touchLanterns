#include "Pause.h"
/*
======================================================================================================================================================
    PAUSE           ENTRY                                       PULSE | TIMER_SET_TO_SHORT           
                    EXIT                                        PULSE | CLEAR_TIMER | FLIP_UP_DOWN   
------------------------------------------------------------------------------------------------------------------------------------------------------
    AUTO            PARENT_IS_AUTO
    FLKR            MEDIUM_TOUCH | NO_TREE                MAKE_TREE
    AUTO            RISING_EDGE | NO_PARENT
    IDLE            TIMER_OVER                                  LEAVE_TREE
------------------------------------------------------------------------------------------------------------------------------------------------------
*/
Pause::Pause() : State( PAUS_ID ) { }

void Pause::enter( Lantern& lantern )
{
    lantern.light.pulse();                      // TODO implement pulse somehow
    lantern.delay.set( SHORT_DELAY );
}
void Pause::exit( Lantern& lantern )
{
    lantern.light.pulse();
    lantern.delay.clear();
    lantern.light.toggleSign();
}
uint8_t Pause::act( Lantern& lantern )
{
    return MAKE_TREE;
}
uint8_t Pause::getNext( Lantern& lantern )
{
    if ( lantern.input == RISING_EDGE and not lantern.parent ) { return AUTO_ID; }
    if ( lantern.parent and *( lantern.parent->state ) == AUTO_ID ) { return AUTO_ID; }
    if ( lantern.input == MEDIUM_TOUCH ) { return FLKR_ID; }
    if ( lantern.delay.isOver() ) { return IDLE_ID; }
    return *this;
}
