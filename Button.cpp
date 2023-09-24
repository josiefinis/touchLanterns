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
  for ( uint8_t i = 0; i < 0x10; i++ ) {
    outputBuffer <<= 2;
    uint8_t held = counter[i];
    switch ( getBit(input) ) {
      case 0:
        if ( not held ) { }
        else if ( held < LONG_INTERVAL ) {
          outputBuffer |= SHORT_PRESS;
        }
        counter[i] = 0;
        break;

      case 1:
        if ( held < LONG_INTERVAL ) {
          counter[i]++;
        }
        else if ( held == LONG_INTERVAL ) {
          outputBuffer |= LONG_PRESS;
          counter[i]++;
        }
        break;
    }
    input <<= 1;
  }
  #if MONITOR_ON
  Serial.print("B->"); Serial.print(outputBuffer, BIN); Serial.print("\t");
  #endif
  return outputBuffer;
}


bool Button::getBit(uint16_t input) { return input >> 0xF; }
