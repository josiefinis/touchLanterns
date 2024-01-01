/*
======================================================================================================================================================
                                SENSOR
======================================================================================================================================================

Controls multiplexer to cycle through capacitive sensor inputs and detect touch.
Uses CapacitiveSensor library.

       -----------------------------------------------------------------------------------------┬------------------------ 5 V
                                                                                                │
                                                                                                │
                                                                           ┌---▄▄---┐           │ 
                Receive (3)-------┬----------------Common Input/Output   1 ┿        ┿ 24  VCC --┘
                                  │      Touch sensor      ○------- I7   2 ┿        ┿ 23  I8  ------○       Touch sensor
                                  █      Touch sensor      ○------- I6   3 ┿        ┿ 22  I9  ------○       Touch sensor
                                  █      Touch sensor      ○------- I5   4 ┿  74HC  ┿ 21  I10 ------○       Touch sensor
                    R1 = 1 kΩ     █      Touch sensor      ○------- I4   5 ┿  4067  ┿ 20  I11 ------○       Touch sensor
                                  █      Touch sensor      ○------- I3   6 ┿        ┿ 19  I12 ------○       Touch sensor
                                  │      Touch sensor      ○------- I2   7 ┿        ┿ 18  I13 ------○       Touch sensor
                  Send (2)--------┘      Touch sensor      ○------- I1   8 ┿        ┿ 17  I14 ------○       Touch sensor
                                         Touch sensor      ○------- I0   9 ┿        ┿ 16  I15 ------○       Touch sensor
                                         Channel control (4)------- S0  10 ┿        ┿ 15  NOT_E ----(GND)
                                         Channel control (5)------- S1  11 ┿        ┿ 14  S2 -------(6) Channel control
                                                               ┌-- GND  12 ┿        ┿ 13  S3 -------(7) Channel control
                                                               │           └--------┘
                                                               │
                                                               │
       --------------------------------------------------------┴--------------------------------------------------------- 0 V
*/
#ifndef SENSOR_H
#define SENSOR_H

#include "Global.h"
#include "Environment.h"
#include <CapacitiveSensor.h>

#define PIN_SENSOR_SEND         2       //                          Arduino Pin 2   ---->   R1:0 
#define PIN_SENSOR_RECEIVE      3       //                          Arduino Pin 3   ---->   R1:1,   4067:1
#define PIN_MUX_S0              0x10    // D4 ( Arduino Pin 4 )     Arduino Pin 4   ---->   4067:10
#define PIN_MUX_S1              0x20    // D5 ( Arduino Pin 5 )     Arduino Pin 5   ---->   4067:11
#define PIN_MUX_S2              0x40    // D6 ( Arduino Pin 6 )     Arduino Pin 6   ---->   4067:14
#define PIN_MUX_S3              0x80    // D7 ( Arduino Pin 7 )     Arduino Pin 7   ---->   4067:13

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
#endif
