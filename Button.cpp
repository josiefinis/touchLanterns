#include "Button.h"
#include "Arduino.h"

#define MONITOR_BUTTON_OUTPUT true
#define LONG_INTERVAL 6   
#define SHORT_PRESS 0b10U
#define LONG_PRESS  0b01U   

Button::Button() { }
    
    
uint32_t Button::output(uint16_t input) {
// Return 16 two-bit output signals based on input and count of immediately preceding consecutive '1' input signals.

  uint32_t outputBuffer = 0;
  for ( uint8_t i=0; i<16; i++ ) {
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
  #if MONITOR_BUTTON_OUTPUT
  Serial.print("\tB "); Serial.print(outputBuffer, HEX); 
  #endif
  return outputBuffer;
}
