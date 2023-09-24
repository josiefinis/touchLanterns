#include "Arduino.h"
#include <CapacitiveSensor.h>

#define PIN_SENSOR_SEND 8
#define PIN_SENSOR_RECEIVE 10
#define HARDWARE_NUMBER_OF_MUX_CHANNELS 16
#define PIN_MUX_S0 0b00000100 // pin 2 -> S0
#define PIN_MUX_S1 0b00001000 // pin 3 -> S1
#define PIN_MUX_S2 0b00010000 // pin 4 -> S2
#define PIN_MUX_S3 0b00100000 // pin 5 -> S3

class Sensor {

  private:
    static uint8_t muxChannel;
    static CapacitiveSensor sensor;
    long sensorInput();
    void advanceMuxChannel();

  public:
    Sensor::Sensor();
    uint16_t output();
};

/* 
NOTE.
The multiplexer control pins are cycled in reflected binary code (RBC, aka Gray code),
whereas the sensor output uses one hot encoding. 
A positive detection on an input channel maps to output as follows:

input     
channel   S3210        output
     0     0000  -->   0000 0000 0000 0001
     2     0010  -->   0000 0000 0000 0010
     3     0011  -->   0000 0000 0000 0100
     1     0001  -->   0000 0000 0000 1000
     5     0101  -->   0000 0000 0001 0000
     7     0111  -->   0000 0000 0010 0000
     6     0110  -->   0000 0000 0100 0000
     4     0100  -->   0000 0000 1000 0000
    12     1100  -->   0000 0001 0000 0000
    14     1110  -->   0000 0010 0000 0000
    15     1111  -->   0000 0100 0000 0000
    13     1101  -->   0000 1000 0000 0000
     9     1001  -->   0001 0000 0000 0000
    11     1011  -->   0010 0000 0000 0000
    10     1010  -->   0100 0000 0000 0000
     8     1000  -->   1000 0000 0000 0000

Multiple detections on a single cycle result in a combined output e.g.
detection on channels 6 and 4 will yield output 0000 0000 1100 0000.
*/
