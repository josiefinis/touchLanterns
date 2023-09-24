#include "Arduino.h"
#include "Sensor.h"

CapacitiveSensor Sensor::sensor = CapacitiveSensor(PIN_SENSOR_SEND, PIN_SENSOR_RECEIVE);
uint8_t Sensor::muxChannel;

Sensor::Sensor() {
}


long Sensor::sensorInput() { 
  #if false 
  Serial.print(sensor.capacitiveSensor(30));
  Serial.print("\t");
  #endif
  return sensor.capacitiveSensor(30);
}


void Sensor::advanceMuxChannel() {
/* Set multiplexer input channel to next in sequence:
  0, 2, 3, 1, 5, 7, 6, 4, 12, 14, 15, 13, 9, 11, 10, 8, 0, ...
*/
  muxChannel++;
  if ( muxChannel % 8 == 0 ) { PORTD ^= PIN_MUX_S3; }
  else if ( muxChannel % 4 == 0 ) { PORTD ^= PIN_MUX_S2; }
  else if ( muxChannel % 2 == 0 ) { PORTD ^= PIN_MUX_S1; }
  else { PORTD ^= PIN_MUX_S0; }
}


uint16_t Sensor::output() {
// Return one-hot encoded sensor output.
  uint16_t hotBit = 1;
  uint16_t sensorOutput = 0;
  while ( hotBit > 0 ) {
    if ( sensorInput() > 80 ) { sensorOutput |= hotBit; }
    advanceMuxChannel();
    hotBit <<= 1;
  }
  return sensorOutput;
}
