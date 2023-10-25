#include "Arduino.h"
#include "Sensor.h"

#define MONITOR_SENSOR_INPUT    false
#define MONITOR_SENSOR_OUTPUT   false
#define SENSOR_SAMPLES          50
#define EDGE_THRESHOLD          80
#define LEVEL_THRESHOLD         25
#define WEIGHT_HISTORY          10
#define EDGE_DETECTION          false

CapacitiveSensor Sensor::sensor = CapacitiveSensor(PIN_SENSOR_SEND, PIN_SENSOR_RECEIVE);


Sensor::Sensor() {
  for ( uint8_t i=0; i<16; i++ ) { baseline[i] = 0xFFFFUL * SENSOR_SAMPLES; }
  i = 0;
  muxChannel = 0;
  sensorOutput = 0;
}


uint16_t Sensor::output() {
// Return one-hot encoded sensor output.
  sensorOutput = 0;
  long rawInput = 0;
  uint16_t normalisedInput = 0;
  uint16_t highestInput = 0;
  do {
    rawInput = input();
    normalisedInput = normalise(rawInput);
    #if MONITOR_SENSOR_INPUT 
    Serial.print(normalisedInput); Serial.print("\t");
    #endif

    if ( not detectLevel(normalisedInput) ) {
      recalibrateBaseline(rawInput);
      advanceMuxChannel();
      continue; 
    }
    if ( normalisedInput < highestInput ) { 
      advanceMuxChannel();
      continue;
    }
    sensorOutput = (1 << muxChannel);
    highestInput = normalisedInput;
    advanceMuxChannel();
  }
  while ( muxChannel % 16 != 0 );

  #if MONITOR_SENSOR_OUTPUT
  Serial.print("\tS "); Serial.println(sensorOutput, HEX);
  #endif
  return sensorOutput;
}


uint8_t Sensor::normalise(long input)  {
  int16_t norm = (input - baseline[muxChannel]) * 10 / SENSOR_SAMPLES;
  return constrain(norm, 0, 255);
}


void Sensor::recalibrateBaseline(long input) {
  baseline[muxChannel] = (WEIGHT_HISTORY * baseline[muxChannel] + input) / (WEIGHT_HISTORY + 1);
  
}


bool Sensor::detectLevel(uint8_t input) {
  return input > LEVEL_THRESHOLD;
}


int8_t Sensor::detectEdge(long input) {
// Return 1 if rising edge, -1 if falling edge, 0 if no edge.
  int16_t difference = input - baseline[muxChannel];
  if ( abs(difference) < EDGE_THRESHOLD ) { 
    return 0; 
  }
  else if ( difference > 0 ) { 
    return 1; 
  }
  else { 
    return -1;
  }
}


void Sensor::zeroOutput() {
// Set the sensor output to 0.
  sensorOutput = 0;
}


long Sensor::input() { 
// Get sensor input using capacitive sensor library.
  return sensor.capacitiveSensorRaw(SENSOR_SAMPLES);
}


void Sensor::advanceMuxChannel() {
// Set multiplexer input channel to next in sequence:
//  0, 2, 3, 1, 5, 7, 6, 4, 12, 14, 15, 13, 9, 11, 10, 8, 0, ...
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


