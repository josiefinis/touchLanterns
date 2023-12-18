/*
======================================================================================================================================================
                                SENSOR
======================================================================================================================================================
*/

#include "Global.h"
#include "Arduino.h"
#include "Random.h"
#include "Sensor.h"

#define NORMALISATION_FACTOR        32
#define LEVEL_THRESHOLD             80
#define WEIGHT_HISTORY              8
#define SENSOR_SAMPLES              32


CapacitiveSensor Sensor::sensor = CapacitiveSensor(PIN_SENSOR_SEND, PIN_SENSOR_RECEIVE);


Sensor::Sensor() {
  for ( uint8_t i=0; i<16; i++ ) { baseline[ i ] = 0xFFFFUL; }
  i = 0;
  muxChannel = 0;
  initialiseBaseline();
}


bool Sensor::output( void ) {
// Return true if sensor detects touch.
  long rawInput = newInput();
  Random::push( rawInput & 0xFF ); 
  Random::seed( rawInput & 0xFF ); 
  uint16_t input = normalise( rawInput );
  
  #if MONITOR_RAW_INPUT 
  Serial.print( rawInput, DEC ); 
  #endif
  #if MONITOR_RAW_INPUT && MONITOR_NORMALISED_INPUT
  Serial.print( " " );
  #endif
  #if MONITOR_NORMALISED_INPUT 
  Serial.print( input, DEC ); 
  #endif
  #if MONITOR_RAW_INPUT || MONITOR_NORMALISED_INPUT
  Serial.print( "\t" );
  #endif

  if ( not isHigh( input ) ) {
    recalibrateBaseline( rawInput );
    return 0;
  }
  return 1;
}


long Sensor::newInput( void ) { 
// Get sensor input using capacitive sensor library.
  return sensor.capacitiveSensorRaw( SENSOR_SAMPLES );
}


uint8_t Sensor::normalise( long rawInput )  {
// Normalise input to lie between 0 and 255. 
  int16_t norm = ( rawInput - baseline[muxChannel] ) * NORMALISATION_FACTOR / SENSOR_SAMPLES;
  return constrain( norm, 0, 255 );
}


bool Sensor::isHigh( uint8_t input ) {
// Return true when the input exceeds the trigger threshold.
  return input > LEVEL_THRESHOLD;
}


uint8_t Sensor::getMuxChannel( void ) {
// Return current multiplexer channel.
  return muxChannel;
}


uint8_t Sensor::nextMuxChannel( void ) {
// Set multiplexer input channel to next in sequence:
//  0, 1, 3, 2, 6, 7, 5, 4, C, D, F, E, A, B, 9, 8, 0, ...
  i++;
  if ( i % 8 == 0 ) { 
    PORTD ^= PIN_MUX_S3; 
    muxChannel ^= 0b1000U;
  }
  else if ( i % 4 == 0 ) { 
    PORTD ^= PIN_MUX_S2; 
    muxChannel ^= 0b0100U;
  }
  else if ( i % 2 == 0 ) { 
    PORTD ^= PIN_MUX_S1; 
    muxChannel ^= 0b0010U;
  }
  else { 
    PORTD ^= PIN_MUX_S0; 
    muxChannel ^= 0b0001U;
  }
  return muxChannel;
}


void Sensor::initialiseBaseline( void ) {
// Calibrate baseline multiple times on all channels for capacitive sensor in 'untouched' state.
  for ( uint8_t i=0; i<0xFF; i++ ) {
    recalibrateBaseline( newInput() );
    nextMuxChannel();
  }
}


void Sensor::recalibrateBaseline( long input ) {
// Calibrate baseline for one channel based on one sensor input.
  baseline[ muxChannel ] = ( WEIGHT_HISTORY * baseline[muxChannel] + input ) / ( WEIGHT_HISTORY + 1 );
}
