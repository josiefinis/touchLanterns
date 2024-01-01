/* 
======================================================================================================================================================
                    MONITOR
======================================================================================================================================================
*/ 
#include "SerialMonitor.h"
#include "Bridge.cpp"

Bridge bridge;

void SerialMonitor::printIndices() 
{
    Serial.println();
    for ( uint8_t i=0; i<16; i++ ) {
        Serial.print( i, HEX ); Serial.print( "\t" );
    }
    Serial.println(); 
    Serial.println();
}


void SerialMonitor::printLine( LanternCollection& collection )
// Print debug data for all lanterns in one line. Format:   "pS dd:  " or "iS bbB  "
{
    Serial.println();
    for ( uint8_t i=0; i<16; ++i )
    {
        Lantern& lantern = collection[ i ];
        uint8_t parent = bridge.getParent( collection, lantern );
        uint8_t delay = bridge.getDelay( lantern );
        uint8_t state = bridge.getState( lantern );
        const SensorInput& input = bridge.getInput( lantern );

        if ( parent != NONE and delay  )
        {
            #if MONITOR_PARENT
                printParent( parent );
            #endif
            #if MONITOR_STATE
                printState( state );
                Serial.print( " " );
            #endif
            #if MONITOR_DELAY
                printDelay( delay );
            #endif
        }
        else {
            #if MONITOR_INPUT
                printInput( input );
            #endif
            #if MONITOR_STATE
                printState( state );
                Serial.print( " " );
            #endif
            #if MONITOR_BRIGHTNESS
                printBrightness( lantern );
            #endif
            #if MONITOR_LIGHT_BEHAVIOUR
                printBehaviour( lantern );
                printSign( lantern );
            #endif
        }
        Serial.print( "\t" );
    }
}


void SerialMonitor::quickPrintLine( LanternCollection& collection )
{
    Serial.println();
    // Faster but less easily readable version.
    for ( uint8_t i=0; i<16; ++i )
    {
        Lantern& lantern = collection[ i ];
        uint8_t parent = bridge.getParent( collection, lantern );
        uint8_t delay = bridge.getDelay( lantern );
        uint8_t state = bridge.getState( lantern );
        const SensorInput& input = bridge.getInput( lantern );

//        if ( parent != NONE and delay  )
//        {
//            #if MONITOR_PARENT
//                printParent( parent );
//            #endif
//            #if MONITOR_STATE
//                if ( state ) { Serial.print( state, HEX ); }
//            #endif
//            #if MONITOR_DELAY
//                printDelay( delay );
//            #endif
//        }
//        else {
            #if MONITOR_INPUT
                if ( input != 1 ) { Serial.print( input, HEX ); }
            #endif
            #if MONITOR_STATE 
                if ( state ) { Serial.print( state, HEX ); }
                Serial.print( " " );
            #endif
            #if MONITOR_BRIGHTNESS
                printBrightness( lantern );
            #endif
            #if MONITOR_LIGHT_BEHAVIOUR
                Serial.print( bridge.getBehaviour( lantern ), HEX )
                Serial.print( bridge.getSign( lantern ), HEX )
            #endif
//        }
        Serial.print( "\t" );
    }
}


void SerialMonitor::printLight( LanternCollection& collection )
{
    Serial.println();
    for ( uint8_t i=0; i<16; ++i )
    {
        Lantern& lantern = collection[ i ];
        uint16_t lightReg = bridge.getLightRegister( lantern );
        Serial.print( lightReg, HEX );
        Serial.print( "\t" );
    }
}


void SerialMonitor::printDelay( uint8_t value ) 
{
    Serial.print( value );
    Serial.print( ":" ); 
}


void SerialMonitor::printParent( uint8_t idx ) 
{
    if ( idx == 0xFF ) { Serial.print( " " ); }
    else       { Serial.print( idx, HEX ); }
}                                                                      



void SerialMonitor::printInput( const SensorInput& input ) 
{
    if ( input == RISING_EDGE  ) { Serial.print( F("↑") );  return; }
    if ( input == FALLING_EDGE ) { Serial.print( F("↓") );  return; }
    if ( input == MEDIUM_TOUCH ) { Serial.print( F("M") );  return; }
    if ( input == LONG_TOUCH   ) { Serial.print( F("L") );  return; }
    Serial.print( " " );
}                                                                      
                                                                       
                                                                       
void SerialMonitor::printRawInput( SensorInput& input ) 
{
    Serial.print( input, HEX );
}


void SerialMonitor::printState( uint8_t stateID ) 
{                                              
    switch ( stateID ) {                                                            
        case IDLE_ID:       Serial.print( F(" ") );        return;
        case WAKE_ID:       Serial.print( F("W") );        return;
        case FULL_ID:       Serial.print( F("F") );        return;
        case FLKR_ID:       Serial.print( F("K") );        return;
        case AUTO_ID:       Serial.print( F("A") );        return;
        case PAUS_ID:       Serial.print( F("P") );        return;
        case PULS_ID:       Serial.print( F("!") );        return;
    }
}


void SerialMonitor::printBehaviour( Lantern& lantern )
{
    switch ( bridge.getBehaviour( lantern ) )
    {
        case TINY_STEP:     Serial.print( F( "t" ) );       return;
        case SMALL_STEP:    Serial.print( F( "s" ) );       return;
        case LARGE_STEP:    Serial.print( F( "L" ) );       return;
        case HUGE_STEP:     Serial.print( F( "H" ) );       return;
        case FLICKER:       Serial.print( F( "f" ) );       return;
        case PULSE:         Serial.print( F( "p" ) );       return;
    }
}


void SerialMonitor::printSign( Lantern& lantern )
{
    if ( not bridge.getBehaviour( lantern ) ) { return; }
    switch ( bridge.getSign( lantern ) )
    {
        case 0:  Serial.print( F( "↑" ) );       return;
        case 1:  Serial.print( F( "↓" ) );       return;
    }
}


void SerialMonitor::printBrightness( Lantern& lantern ) 
{                                              
    if ( lantern.getBrightness() )
    {
        Serial.print( lantern.getBrightness(), HEX );
    }
}


void SerialMonitor::printPWM( PWMSignal& pwm )
{
    pwm.edgeQueue.print();
}
