/*
======================================================================================================================================================
                                      REGISTER
======================================================================================================================================================

Writes to hardware SIPO shift register.

                                               ┌--------┐             
         Parallel data output          Q1    1 ┿ 〇     ┿ 16  VCC
         Parallel data output          Q2    2 ┿        ┿ 15  Q0            Parallel data output
         Parallel data output          Q3    3 ┿        ┿ 14  DS            Serial data input
         Parallel data output          Q4    4 ┿  74HC  ┿ 13  NOT_OE        Output enable ( active LOW )
         Parallel data output          Q5    5 ┿  595   ┿ 12  ST_CP         Storage register clock pulse
         Parallel data output          Q6    6 ┿        ┿ 11  SH_CP         Shift register Clock pulse
         Parallel data output          Q7    7 ┿        ┿ 10  NOT_MR        Master reset ( active LOW )
                                      GND    8 ┿        ┿ 9   Q7'           Serial data output
                                               └--------┘
*/

#ifndef REGISTER_H
#define REGISTER_H

#include "Global.h"
#include "Environment.h"

#define PIN_REGISTER_DS         0x01    // B0 (Arduino pin 8)       Arduino pin 8  ---> 595 pin 14
#define PIN_REGISTER_NOT_OE     0x02    // B1 (Arduino pin 9)       Arduino pin 9  ---> 595 pin 13
#define PIN_REGISTER_NOT_MR     0x04    // B2 (Arduino pin 10)      Arduino pin 10 ---> 595 pin 10
#define PIN_REGISTER_ST_CP      0x08    // B3 (Arduino pin 11)      Arduino pin 11 ---> 595 pin 12
#define PIN_REGISTER_SH_CP      0x10    // B4 (Arduino pin 12)      Arduino pin 12 ---> 595 pin 11


class Register {
    public:
        Register();

        void reset( void );
        void write( uint16_t word );

    private:
        void writeToShiftRegister( uint16_t word );
        void copyToStorageRegister();
        void writeBit( bool bit );
        void clearShiftRegister( void );
        void enableOutput( void );
        void disableOutput( void );
        void shiftRegisterClockPulse( void );
        void storageRegisterClockPulse( void );
};
#endif
