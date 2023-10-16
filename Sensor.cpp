#include "Arduino.h"
#include "Sensor.h"

#define LIMITED_MONITOR_ON false
#define SENSOR_SAMPLES 30
#define EDGE_THRESHOLD 80


CapacitiveSensor Sensor::sensor = CapacitiveSensor(PIN_SENSOR_SEND, PIN_SENSOR_RECEIVE);

Sensor::Sensor() {
  i = 0;
  muxChannel = 0;
  sensorOutput = 0;
}


uint16_t Sensor::output() {
// Return one-hot encoded sensor output.
  long newInput;
  do {
    newInput = input();
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
    lastInput[muxChannel] = newInput;
    advanceMuxChannel();
  }
  while ( muxChannel % 16 != 0 );

  #if LIMITED_MONITOR_ON
  Serial.print(sensorOutput, BIN); Serial.print("\n");
  #endif
  return sensorOutput;
}


int8_t Sensor::detectEdge(long input) {
// Return 1 if rising edge, -1 if falling edge, 0 if no edge.
  int16_t difference = input - lastInput[muxChannel];
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
  #if LIMITED_MONITOR_ON 
  Serial.print(sensor.capacitiveSensor(SENSOR_SAMPLES)); Serial.print("\t");
  #endif
  return sensor.capacitiveSensor(SENSOR_SAMPLES);
}


void Sensor::advanceMuxChannel() {
// Set multiplexer input channel to next in sequence:
//  0, 2, 3, 1, 5, 7, 6, 4, 12, 14, 15, 13, 9, 11, 10, 8, 0, ...
  i++;
  if ( i % 8 == 0 ) { 
    PORTD ^= PIN_MUX_S3; 
    muxChannel ^= 0b1000;
  }
  else if ( i % 4 == 0 ) { 
    PORTD ^= PIN_MUX_S2; 
    muxChannel ^= 0b0100;
  }
  else if ( i % 2 == 0 ) { 
    PORTD ^= PIN_MUX_S1; 
    muxChannel ^= 0b0010;
  }
  else { 
    PORTD ^= PIN_MUX_S0; 
    muxChannel ^= 0b0001;
  }
}


