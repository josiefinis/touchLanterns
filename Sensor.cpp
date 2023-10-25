#include "Arduino.h"
#include "Sensor.h"

#define MONITOR_RAW_INPUT           false
#define MONITOR_NORMALISED_INPUT    false
#define MONITOR_SENSOR_OUTPUT       false
#define SENSOR_SAMPLES              50
#define NORMALISATION_FACTOR        40
#define LEVEL_THRESHOLD             100
#define WEIGHT_HISTORY              10

CapacitiveSensor Sensor::sensor = CapacitiveSensor(PIN_SENSOR_SEND, PIN_SENSOR_RECEIVE);


Sensor::Sensor() {
  for ( uint8_t i=0; i<16; i++ ) { baseline[i] = 0xFFFFUL; }
  i = 0;
  muxChannel = 0;
  sensorOutput = 0;
  initialiseBaseline();
}


uint16_t Sensor::output() {
// Return one-hot encoded sensor output.
  sensorOutput = 0;
  long rawInput = 0;
  uint16_t input = 0;
  uint16_t highestInput = 0;
  do {
    rawInput = newInput();
    input = normalise(rawInput);
    #if MONITOR_RAW_INPUT 
    Serial.print(rawInput); Serial.print("\t");
    #endif
    #if MONITOR_NORMALISED_INPUT 
    Serial.print(input); Serial.print("\t");
    #endif

    if ( not isHigh(input) ) {
      recalibrateBaseline(rawInput);
      incrementMuxChannel();
      continue; 
    }
    if ( input < highestInput ) { 
      incrementMuxChannel();
      continue;
    }
    sensorOutput = (1 << muxChannel);
    highestInput = input;
    incrementMuxChannel();
  }
  while ( muxChannel % 16 != 0 );

  #if MONITOR_SENSOR_OUTPUT
  Serial.print("\tS "); Serial.println(sensorOutput, HEX);
  #endif
  return sensorOutput;
}


long Sensor::newInput() { 
// Get sensor input using capacitive sensor library.
  return sensor.capacitiveSensorRaw(SENSOR_SAMPLES);
}


uint8_t Sensor::normalise(long rawInput)  {
// Normalise input to lie between 0 and 255. 
  int16_t norm = (rawInput - baseline[muxChannel]) * NORMALISATION_FACTOR / SENSOR_SAMPLES;
  return constrain(norm, 0, 255);
}


bool Sensor::isHigh(uint8_t input) {
// Return true when the input exceeds the trigger threshold.
  return input > LEVEL_THRESHOLD;
}


void Sensor::incrementMuxChannel() {
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
}


void Sensor::initialiseBaseline() {
// Calibrate baseline multiple times on all channels for capacitive sensor in 'untouched' state.
  for ( uint8_t i=0; i<0xFF; i++ ) {
    recalibrateBaseline(newInput());
    incrementMuxChannel();
  }
}


void Sensor::recalibrateBaseline(long input) {
// Calibrate baseline for one channel based on one sensor input.
  baseline[muxChannel] = (WEIGHT_HISTORY * baseline[muxChannel] + input) / (WEIGHT_HISTORY + 1);
}


void Sensor::zeroOutput() {
// Set the sensor output to 0.
  sensorOutput = 0;
}
