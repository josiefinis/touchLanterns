/*
======================================================================================================================================================
                    STATE
======================================================================================================================================================
*/

#include "Arduino.h"
#include "State.h"
#include "Light.h"


State::State( const uint8_t id )
    : id( id )
{ }

State::operator int() const { return id; }

//bool State::operator == ( const State& other )   const { return id == other.id; }
//bool State::operator == ( const uint8_t& other ) const { return id == other; }
//bool State::operator != ( const State& other )   const { return !( *this == other.id ); }
//bool State::operator != ( const uint8_t& other ) const { return !( *this == other ); }

void    State::enter( Lantern& lantern ) {}
void    State::exit( Lantern& lantern ) {}
uint8_t State::act( Lantern& lantern ) { return 0; }
uint8_t State::getNext( Lantern& lantern ) { return 0; } 


/*



            IDLE ----> WAKE ----->


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
        lantern.delay = 0;
        lantern.reference = 0;
        return 0;
    }
    if ( lantern.delay == 0 and lantern.light < lantern.reference )
    {
        lantern.delay = 0xFF;   
        lantern.light.raiseBrightness();
        return 0;
    }
    if ( lantern.delay == 0 and lantern.light > lantern.reference )
    {
        lantern.delay = 0xFF;   
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
        lantern.delay = Random::uRandN( 16 );
        lantern.light.setSign( lantern.parent->light.getSign() );
        return FULL_ID;
    }
    if ( *( lantern.parent->state ) == AUTO_ID and not lantern.parent->delay )
    {
        lantern.delay = Random::uRandN( 16 );
        lantern.light.setSign( lantern.parent->light.getSign() );
        return AUTO_ID;
    }
    return *this;
}
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
void Wake::enter( Lantern& lantern ) 
{ 
    if ( lantern.getBrightness() < 32 ) { lantern.light.setPositiveRate(); }
    else { lantern.light.setNegativeRate(); }
    lantern.light.setRate( 5 ); 
}

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
Full::Full() : State( FULL_ID ) { }

// Set rate to highest possible for simple on/off effect.
void Full::enter( Lantern& lantern ){ lantern.light.setRate( 7 ); }

void Full::exit( Lantern& lantern ) { lantern.light.setRate( 0 ); }

// Change brightness according to up/down bit, or if following a parent, towards it. 
uint8_t Full::act( Lantern& lantern )
{
    if ( lantern.parent and lantern.delay ) 
    { 
        return 0;
    }
    lantern.light.changeBrightness(); 
    lantern.light.changeBrightness(); 
    return 0;
}

// Change to IDLE state once fully lit or fully out.
uint8_t Full::getNext( Lantern& lantern )
{
    if ( lantern.parent and lantern.delay )
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
Auto::Auto() : State( AUTO_ID ) { }

void Auto::enter( Lantern& lantern ) { lantern.light.setRate( 1 ); }

void Auto::exit( Lantern& lantern ) { } 

uint8_t Auto::act( Lantern& lantern )
{
    if ( not lantern.parent ) 
    {
        lantern.light.changeBrightness( 1, LIGHT_FULL );
        return 0;
    }
    if ( lantern.delay ) { return 0; }
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
    lantern.delay = SHORT_DELAY;
}
void Pause::exit( Lantern& lantern )
{
    lantern.light.pulse();
    lantern.delay = 0;
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
    if ( lantern.delay == 0 ) { return IDLE_ID; }
    return *this;
}
