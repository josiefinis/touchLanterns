#include "Arduino.h"
#include "Sensor.h"

#define MONITOR_ON false
#define SENSOR_THRESHOLD 250
#define SENSOR_SAMPLES 30


CapacitiveSensor Sensor::sensor = CapacitiveSensor(PIN_SENSOR_SEND, PIN_SENSOR_RECEIVE);

Sensor::Sensor() {
  i = 0;
  muxChannel = 0;
}


long Sensor::input() { 
// Get sensor input using capacitive sensor library.
  #if MONITOR_ON 
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


uint16_t Sensor::output() {
// Return one-hot encoded sensor output.
  uint16_t sensorOutput = 0;
  do {
    advanceMuxChannel();
    if ( input() > SENSOR_THRESHOLD ) { 
      sensorOutput |= (1 << muxChannel);
    }
  }
  while ( muxChannel % 16 != 0 ) 
  #if MONITOR_ON
  Serial.println();
  #endif
  return sensorOutput;
}
