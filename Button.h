/* Button class. 

Convert input signal from sensor to output button action signal.

    input: one-hot encoding of output from 16 sensors. 
              0 = no detection, 
              1 = detection.

    output: 16 two-bit encoding of button actions:
              00 = no action,
              10 = short press,
              11 = long press.
*/

#include "Arduino.h"

class Button {
 private:
  
  uint8_t counter[16]; 

 public:

  Button();
  uint32_t output(uint16_t input);
};
