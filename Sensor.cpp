#include "Arduino.h"
#include "Sensor.h"

#define MONITOR_SENSOR_INPUT    true
#define MONITOR_SENSOR_OUTPUT   true
#define SENSOR_SAMPLES          100
#define EDGE_THRESHOLD          80
#define LEVEL_THRESHOLD         30
#define WEIGHT_HISTORY          10
#define EDGE_DETECTION          false

CapacitiveSensor Sensor::sensor = CapacitiveSensor(PIN_SENSOR_SEND, PIN_SENSOR_RECEIVE);


Sensor::Sensor() {
  for ( uint8_t i=0; i<16; i++ ) { 
    baseline[i] = 0xFFFFUL * SENSOR_SAMPLES; 
    highline[i] = 0xFFFFUL * SENSOR_SAMPLES; 
  }
  i = 0;
  muxChannel = 0;
  sensorOutput = 0;
}


uint16_t Sensor::output() {
// Return one-hot encoded sensor output.
  int32_t newInput;
  do {
    newInput = input();

    #if EDGE_DETECTION
    switch ( detectEdge(newInput) ) {
      case 0:
        break;
      case 1:
        sensorOutput |= (1 << muxChannel);
        break;
      case -1:
        sensorOutput &= ~(1 << muxChannel);
        break;
    }
    baseline[muxChannel] = newInput;
    #if MONITOR_SENSOR_INPUT 
    Serial.print(newInput); Serial.print("\t");
    #endif

    #else
    if ( detectLevel(normalise(newInput)) ) {
      sensorOutput |= (1 << muxChannel);
    }
    else {
      sensorOutput &= ~(1 << muxChannel);
      adjustBaseline(newInput);
    }
    adjustHighline(newInput);
    #if MONITOR_SENSOR_INPUT 
    Serial.print(normalise(newInput)); Serial.print("\t");
    #endif

    #endif

    // TODO if all else fails, only consider channel with highest input for each cycle. 
    advanceMuxChannel(); // TODO try adding a short wait after switching channel.
  }
  while ( muxChannel % 16 != 0 );

  #if MONITOR_SENSOR_OUTPUT
  Serial.print("\tS "); Serial.print(sensorOutput, HEX);
  #endif
  return sensorOutput;
}


uint8_t Sensor::normalise(int32_t input)  {
// Normalise the input to be somewhere between 0 and 100;
  int16_t norm = (input - baseline[muxChannel]) * 100 / highline[muxChannel];
  return constrain(norm, 0, 100);
}


void Sensor::adjustBaseline(int32_t input) {
// Adjust the baseline towards the current input using a weighted average.
  baseline[muxChannel] = (WEIGHT_HISTORY * baseline[muxChannel] + input) / (WEIGHT_HISTORY + 1);
}


void Sensor::adjustHighline(int32_t input) {
// Set the highline to the highest recorded value over the baseline. Adjust the highline so that it tracks drift in the baseline.
  int16_t diff = input - baseline[muxChannel];
  if ( diff > highline[muxChannel] ) {
    highline[muxChannel] = diff;
  }
  else if ( diff < SENSOR_SAMPLES / 8 ) {
    highline[muxChannel] += diff;
  }
  return 0;
}


bool Sensor::detectLevel(uint8_t input) {
  return input > LEVEL_THRESHOLD;
}


int8_t Sensor::detectEdge(int32_t input) {
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


int32_t Sensor::input() { 
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


