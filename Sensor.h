/*
======================================================================================================================================================
                                SENSOR
======================================================================================================================================================
*/
#ifndef SENSOR_H
#define SENSOR_H

#include "Arduino.h"
#include <CapacitiveSensor.h>

class Sensor {
  public:
    Sensor();
    uint8_t getMuxChannel( void );
    uint8_t nextMuxChannel( void );
    bool output( void );

  private:
    static CapacitiveSensor sensor;
    uint16_t baseline[ 16 ];
    uint8_t i;
    uint8_t muxChannel;

    long newInput( void );
    uint8_t normalise( long input );
    bool isHigh( uint8_t input );
    void initialiseBaseline( void );
    void recalibrateBaseline( long input );
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
#endif
