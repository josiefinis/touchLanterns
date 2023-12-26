/*
======================================================================================================================================================
    IDLE            ENTRY                                       STEP_SET_TO_TINY
                    EXIT                                        TIMER_CLEAR
------------------------------------------------------------------------------------------------------------------------------------------------------
                    LIGHT_EQ_ZERO                               TIMER_CLEAR | REF_SET_TO_ZERO
                    TIMER_OVER | LIGHT_GT_REF                   TIMER_SET_LONG, LIGHT_LOWER
                    TIMER_OVER | LIGHT_LT_REF                   TIMER_SET_LONG, LIGHT_RAISE
    AUTO            PARENT_IS_AUTO                              TIMER_SET_TO_RANDOM
    FULL            PARENT_IS_FULL                              TIMER_SET_TO_RANDOM
    WAKE            RISING_EDGE | NO_PARENT
------------------------------------------------------------------------------------------------------------------------------------------------------
*/

#include "Idle.h"


// IDLE State: either unlit or lit, if lit trending slowly towards reference brightness.
Idle::Idle() : State( IDLE_ID ) { }

// Set slowest rate of change on entry ( step size = 2^rate ).
void Idle::enter( Lantern& lantern ) 
{ 
    lantern.light.setRate( 0 );
    lantern.reference = lantern.light;
}

// Clear the delay trigger so it won't cause an action in error.  // TODO delete comment 
void Idle::exit( Lantern& lantern )  { }

// Trend slowly toward reference brightness, which itself meanders slowly downwards, 
// so that the lights burn lower and lower until they go out.
uint8_t Idle::act( Lantern& lantern )
{
    if ( lantern.light == 0 )
    {
        lantern.delay.clear();
        lantern.reference = 0;
        return 0;
    }
    if ( lantern.delay.isOver() and lantern.light < lantern.reference )
    {
        lantern.delay.set( 0xFF );   
        lantern.light.raiseBrightness();
        return 0;
    }
    if ( lantern.delay.isOver() and lantern.light > lantern.reference )
    {
        lantern.delay.set( 0xFF );   
        lantern.light.lowerBrightness();
        return 0;
    }
    return 0;
}

// Change state immediately on touch or if given a parent lantern to follow.
uint8_t Idle::getNext( Lantern& lantern )
{
    if ( lantern.input == RISING_EDGE and not lantern.parent ) { return WAKE_ID; }
    if ( not lantern.parent ) { return *this; }

    if ( *( lantern.parent->state ) == FULL_ID and not lantern.parent->delay )
    {
        lantern.delay.setRandom( 3 );
        lantern.light.setSign( lantern.parent->light.getSign() );
        return FULL_ID;
    }
    if ( *( lantern.parent->state ) == AUTO_ID and not lantern.parent->delay )
    {
        lantern.delay.setRandom( 3 );
        lantern.light.setSign( lantern.parent->light.getSign() );
        return AUTO_ID;
    }
    return *this;
}
