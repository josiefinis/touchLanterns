#include "Button.h"
#include "Arduino.h"

#define MONITOR_ON false
#define LONG_INTERVAL 5
#define SHORT_PRESS 0b10
#define LONG_PRESS  0b11

Button::Button() { }
    
    
uint32_t Button::output(uint16_t input) {
// Return 16 two-bit output signals based on input and count of immediately preceding consecutive '1' input signals.

  uint32_t outputBuffer = ~0;
  for ( uint8_t i=0; i<16; i++ ) {
    uint8_t held = counter[i];
    switch ( input >> i & 1 ) {
      case 0:
        counter[i] = 0;
        break;

      case 1:
        if ( counter[i] == 0 ) {
          outputBuffer |= (SHORT_PRESS << 2*i);
        }
        if ( counter[i] < LONG_INTERVAL ) {
          counter[i]++;
        }
        else if ( counter[i] == LONG_INTERVAL ) {
          outputBuffer |= (LONG_PRESS << 2*i);
          counter[i]++;
        }
        break;
    }
  }
  #if MONITOR_ON
  Serial.print("B->"); Serial.print(outputBuffer, BIN); Serial.print("\t");
  #endif
  return outputBuffer;
}
