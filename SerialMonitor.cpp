
/* 
======================================================================================================================================================
                    MONITOR
======================================================================================================================================================
*/ 
#include "SerialMonitor.h"
#include "Bridge.cpp"

Bridge bridge;

void SerialMonitor::printLine( LanternCollection& collection )
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
            printState( state );
            printDelay( delay );
        }
        else {
            printInput( input );
            printBrightness( lantern );
            Serial.print( "\t" );
        }
    }
}


void SerialMonitor::printDelay( uint8_t value ) 
{
    Serial.print( ":" ); Serial.print( value );
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
        case IDLE_ID:      Serial.print( F("  ") );        return;
        case WAKE_ID:      Serial.print( F("W ") );        return;
        case FULL_ID:      Serial.print( F("F ") );        return;
        case FLKR_ID:      Serial.print( F("K ") );        return;
        case AUTO_ID:      Serial.print( F("A ") );        return;
        case PAUS_ID:      Serial.print( F("P ") );        return;
    }
}


void SerialMonitor::printBrightness( Lantern& lantern ) 
{                                              
    Serial.print( lantern.getBrightness(), HEX );
}

                                                                                
void SerialMonitor::printIndices() {
    Serial.println();
    for ( uint8_t i=0; i<16; i++ ) {
        Serial.print( i, HEX ); Serial.print( "\t" );
    }
    Serial.println(); 
    for ( uint8_t i=0; i<16; i++ ) {
        Serial.print( F( "========" ) );
    }
    Serial.println();
}


//void printLantern( Lantern& lantern ) {
//  #if MONITOR_INPUT
//    Serial.print( lantern.input( idx ), HEX ); 
//  #endif
//  #if MONITOR_STATE
//    Serial.print( "s" ); 
//    Serial.print( lantern.getState( idx ), HEX ); 
//  #endif
//  #if MONITOR_OUTPUT
//    Serial.print( "o" ); 
//    Serial.print( lantern.getOutput( idx ), HEX ); 
//  #endif
//}
//
//
//void printLine() {
//  bool chosenIndex = 0xF;
//  bool all = 1;
//  Serial.println();
//  for ( uint8_t i = 0; i < 16; i++ ) { 
//    if ( all ) { 
//      uint8_t& idx = i;
//      printLantern( idx );
//    }
//    if ( not all and i == 0 ) {
//      uint8_t idx = chosenIndex;
//      printLantern( idx );
//    }
//    #if MONITOR_BRIGHTNESS
//      Serial.print( "b" ); 
//      Serial.print( lantern.getBrightness( idx ) ); 
//    #endif
//    Serial.print( "\t" );
//  }
//}
//#endif
//
//
//#if MONITOR_PARENT
//void printParent() {
//  for ( uint8_t i=0; i<16; i++ ) { 
//    if ( lantern.getParent( i ) ) {
//      Serial.print( lantern[i].getParent()->getIndex(), HEX ); 
//    }
//    Serial.print( "\t" );
//  }
//  Serial.println();
//}
//#endif
//
//
//#if MONITOR_DELAY
//void printDelay() {
//  Serial.println();
//  for ( uint8_t i=0; i<16; i++ ) { Serial.print( lantern.getDelay( i ) ); Serial.print( " ds\t" ); }
//}
//#endif
//
//
//#if MONITOR_REGISTER_SIGNAL
//void printRegisterSignal() {
//  Serial.print("\nInterval\tSignal\n"); 
//  for ( uint8_t i=0; i<32; i++ ) {
//    //Serial.print(microsMONITOR[MONITORidx & 0x1F], BIN); Serial.print("\t");
//    Serial.print(edgeAtMicrosMONITOR[MONITORidx & 0x1F]); Serial.print("\t");
//    Serial.print(signalMONITOR[MONITORidx & 0x1F], BIN); Serial.print("\n");
//    MONITORidx++;
//  }
//}
//#endif
//
//
//#if MONITOR_TIMINGS
//void printTimings() {
//  Serial.print("Sensor input:\t"); 
//  Serial.print(inputTimer.getMean()); Serial.print("\t"); 
//  Serial.println(inputTimer.getMax());
//  Serial.print("Lantern update:\t"); 
//  Serial.print(updateTimer.getMean()); Serial.print("\t"); 
//  Serial.println(updateTimer.getMax());
//  Serial.print("Brightness:\t"); 
//  Serial.print(brightnessTimer.getMean()); Serial.print("\t"); 
//  Serial.println(brightnessTimer.getMax());
//  Serial.println();
//}
//#endif
