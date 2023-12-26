#include "Wake.h"
/*
======================================================================================================================================================
    WAKE            ENTER                                       STEP_SET_TO_LARGE
------------------------------------------------------------------------------------------------------------------------------------------------------
                    ALWAYS                                      LIGHT_RAISE
                    STEP_IS_DOWN                                LIGHT_LOWER
    FULL            FALLING_EDGE
    FLKR            MEDIUM_TOUCH
------------------------------------------------------------------------------------------------------------------------------------------------------
*/


// WAKE state is the first state entered on touch, before branching depending on if touch is held or released.
Wake::Wake() : State( WAKE_ID ) { }

// Set a large rate in order to give immediate and obviousvisual feedback.
void Wake::enter( Lantern& lantern ) { lantern.light.setRate( 5 ); }

void Wake::exit( Lantern& lantern )  { }

// Change brightness until next state.
uint8_t Wake::act( Lantern& lantern )
{ 
    lantern.light.changeBrightness(); 
    return 0;
} //                                        TODO make different for down

// Turn brightness fully up or fully off on a brief touch, which will be the end of this branch.
// If held, continue on a path that branches further.
uint8_t Wake::getNext( Lantern& lantern )
{
    if ( lantern.input == FALLING_EDGE ) { return FULL_ID; }
    if ( lantern.input == MEDIUM_TOUCH ) { return FLKR_ID; }
    return *this;
}
