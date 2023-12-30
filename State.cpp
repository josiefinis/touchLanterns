/*
======================================================================================================================================================
                    STATE
======================================================================================================================================================
*/

#include "State.h"
#include "Light.h"
#include "Random.h"


State::State( const uint8_t id )
    : id( id )
{ }

State::operator int() const { return id; }

void    State::enter( Lantern& lantern ) {}
void    State::exit( Lantern& lantern ) {}
uint8_t State::act( Lantern& lantern ) { return 0; }
uint8_t State::getNext( Lantern& lantern ) { return 0; } 


/*

            Sensor input, i in { RISING_EDGE (↑), FALLING_EDGE (↓), MEDIUM_TOUCH (M), LONG_TOUCH (L) }
            Brightness, b in   [ 0, 255 (full) ]
            Delay, d in [ 0, 255 ]



                           i==M-----------------------------------┐                                                      i==M----------------------------------┐
                           │                                      ↓                                                      │                                     ↓ 
    ┏━━━━━━┓           ┏━━━━━━┓            ┏━━━━━━┓           ┏━━━━━━┓           ┏━━━━━━┓                            ┏━━━━━━┓           ┏━━━━━━┓           ┏━━━━━━┓
    ┃ IDLE ┃---i==↑--->┃ WAKE ┃----i==↓--->┃ FULL ┃<---i==L---┃ FLKR ┃---i==↓--->┃ AUTO ┃---i==↑ | b==1 | b==full--->┃ PAUS ┃---i==↑--->┃ PULS ┃           ┃ RIPL ┃ 
    ┗━━━━━━┛           ┗━━━━━━┛            ┗━━━━━━┛           ┗━━━━━━┛           ┗━━━━━━┛                            ┗━━━━━━┛           ┗━━━━━━┛           ┗━━━━━━┛
       ↑                                      │                                     ↑                                   │                  │                  │
       └------------b==0 | b==full------------┘                                     └----------------------------------)│(-----------------┘<---------------d==0
       ↑                                                                                                                │
       └--------------------------------------------------------------------------------------------------------------d==0
      




------------------------------------------------------------------------------------------------------------------------------------------------------
                    IDLE           
------------------------------------------------------------------------------------------------------------------------------------------------------
*/

// IDLE State: either unlit or lit, if lit trending slowly towards reference brightness.
Idle::Idle() : State( IDLE_ID ) { }

// Set slowest rate of change on entry ( step size = 2^rate ).
void Idle::enter( Lantern& lantern ) 
{ 
    lantern.light.setBehaviour( TINY_STEP );
    lantern.reference = lantern.light - 2;
}

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

    lantern.light.setSign( Random::pull( 1 ) );
    lantern.light.setBehaviour( Random::pull( 2 ) );
    if ( lantern.light < lantern.reference )
    {
        lantern.light.changeBrightness( 0, lantern.reference - 2 );
        return 0;
    }
    if ( lantern.light > lantern.reference )
    {
        lantern.light.changeBrightness( lantern.reference + 2, LIGHT_FULL );
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
    if ( *( lantern.parent->state ) == RIPL_ID )
    {
        lantern.light.setSign( lantern.parent->light.getSign() );
        return RIPL_ID;
    }
    return *this;
}


/*
------------------------------------------------------------------------------------------------------------------------------------------------------
                    WAKE
------------------------------------------------------------------------------------------------------------------------------------------------------
*/
// WAKE state is the first state entered on touch, before branching depending on if touch is held or released.
Wake::Wake() : State( WAKE_ID ) { }

// Set a large rate in order to give immediate and obviousvisual feedback.
void Wake::enter( Lantern& lantern ) 
{ 
    if ( lantern.getBrightness() < 64 ) { lantern.light.setToBrighten(); }
    else { lantern.light.setToDim(); }
    lantern.light.setBehaviour( LARGE_STEP ); 
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
------------------------------------------------------------------------------------------------------------------------------------------------------
                    FULL
------------------------------------------------------------------------------------------------------------------------------------------------------
*/
// FULL state, turn brightess fully up or off.
Full::Full() : State( FULL_ID ) { }

// Set rate to highest possible for simple on/off effect.
void Full::enter( Lantern& lantern ){ lantern.light.setBehaviour( HUGE_STEP ); }

void Full::exit( Lantern& lantern ) { }

// Change brightness according to up/down bit, or if following a parent, towards it. 
uint8_t Full::act( Lantern& lantern )
{
    if ( lantern.parent and lantern.delay ) 
    { 
        return 0;
    }
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
                    FLKR
------------------------------------------------------------------------------------------------------------------------------------------------------
*/
// FLKR state is a branching state 
Flicker::Flicker() : State( FLKR_ID ) { }

void Flicker::enter( Lantern& lantern )
{
    lantern.light.setBehaviour( FLICKER );
    lantern.reference = lantern.light;
}

void Flicker::exit( Lantern& lantern ) 
{ 
    lantern.light.setBehaviour( STABLE );
    lantern.light.setBrightness( lantern.reference ); 
}

uint8_t Flicker::act( Lantern& lantern )
{ 
    if ( lantern.input == LONG_TOUCH )
    {
        return MAKE_TREE;
    }
    lantern.light.changeBrightness(); 
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
                    AUTO
------------------------------------------------------------------------------------------------------------------------------------------------------
*/
Auto::Auto() : State( AUTO_ID ) { }

void Auto::enter( Lantern& lantern ) { lantern.light.setBehaviour( SMALL_STEP ); }

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
        lantern.light.setToDim();
        lantern.light.changeBrightness( lantern.parent->light, lantern.parent->light ); 
        return 0;
    }
    if ( lantern.light < lantern.parent->light )  
    { 
        lantern.light.setToBrighten();
        lantern.light.changeBrightness( lantern.parent->light, lantern.parent->light ); 
        return 0;
    }
    return 0;
}

uint8_t Auto::getNext( Lantern& lantern )
{
    if ( lantern.light == 1 ) { return PAUS_ID; }
    if ( lantern.light == LIGHT_FULL ) { return PAUS_ID; }
    if ( lantern.input == RISING_EDGE and not lantern.parent ) { return PAUS_ID; }
    if ( not lantern.parent ) { return *this; }
    if ( lantern.light == lantern.parent->light and *( lantern.parent->state ) != AUTO_ID ) { return PAUS_ID; }
    return *this;
}


/*
------------------------------------------------------------------------------------------------------------------------------------------------------
                    PAUSE
------------------------------------------------------------------------------------------------------------------------------------------------------
*/
Pause::Pause() : State( PAUS_ID ) { }

void Pause::enter( Lantern& lantern )
{
    lantern.reference = lantern.light;
    lantern.light.setBehaviour( PULSE );
    lantern.delay = SHORT_DELAY;
}


void Pause::exit( Lantern& lantern )
{
    lantern.light.toggleSign();
    lantern.delay = 0;
}


uint8_t Pause::act( Lantern& lantern ) 
{ 
    lantern.light.changeBrightness();
    if ( lantern.input == MEDIUM_TOUCH )    { return MAKE_TREE; }
    if ( lantern.delay == 0 )               { lantern.parent = nullptr; }
    return 1;
}


uint8_t Pause::getNext( Lantern& lantern )
{
    if ( lantern.input == RISING_EDGE and not lantern.parent )      { return PULS_ID; }
    if ( lantern.parent and *( lantern.parent->state ) == AUTO_ID ) { return AUTO_ID; }
    if ( lantern.input == MEDIUM_TOUCH )                            { return RIPL_ID; }
    if ( lantern.delay == 0 )                                       { return IDLE_ID; }
    return *this;
}


/*
------------------------------------------------------------------------------------------------------------------------------------------------------
                        PULSE
------------------------------------------------------------------------------------------------------------------------------------------------------
*/
Pulse::Pulse() : State( PULS_ID ) { }

void Pulse::enter( Lantern& lantern )
{
    lantern.light.setBehaviour( PULSE );
    lantern.reference = lantern.light;
}


void Pulse::exit( Lantern& lantern ) { }


uint8_t Pulse::act( Lantern& lantern ) 
{ 
    lantern.light.changeBrightness();
    return 1;
}


uint8_t Pulse::getNext( Lantern& lantern )
{
    if ( lantern.light.getBehaviour() == STABLE ) { return AUTO_ID; }
    return *this;
}


/*
------------------------------------------------------------------------------------------------------------------------------------------------------
                    RIPPLE
------------------------------------------------------------------------------------------------------------------------------------------------------
*/
Ripple::Ripple() : State( RIPL_ID ) { }

void Ripple::enter( Lantern& lantern ) 
{ 
    lantern.reference = 128;
    lantern.light.setBehaviour( RIPPLE ); 
    lantern.delay = 8;
}


void Ripple::exit( Lantern& lantern ) { }


uint8_t Ripple::act( Lantern& lantern ) 
{ 
    lantern.light.changeBrightness();
    return 1;
}


uint8_t Ripple::getNext( Lantern& lantern )
{
    if ( lantern.delay == 0 ) { return AUTO_ID; }
    return *this;
}
