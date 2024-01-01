/*
======================================================================================================================================================
                                      REGISTER
======================================================================================================================================================

Writes to hardware 16 bit SIPO shift register.

    ----------------------------------------------------------------------------------┬----------------------------------------------- 5 V
                                                                                      │
                                                            ┌--------┐                │
         Parallel data output ( light 1 ) ---------- Q1   1 ┿ 〇     ┿ 16   VCC ------┤
         Parallel data output ( light 2 ) ---------- Q2   2 ┿        ┿ 15   Q0 -------│---- ( light 0 )   Parallel data output
         Parallel data output ( light 3 ) ---------- Q3   3 ┿        ┿ 14   DS -------│---- (8)           Serial data input
         Parallel data output ( light 4 ) ---------- Q4   4 ┿  74HC  ┿ 13   NOT_OE ---│---- (9)           Output enable ( active LOW )
         Parallel data output ( light 5 ) ---------- Q5   5 ┿  595   ┿ 12   ST_CP ----│---- (11)          Storage register clock pulse
         Parallel data output ( light 6 ) ---------- Q6   6 ┿        ┿ 11   SH_CP ----│---- (12)          Shift register Clock pulse
         Parallel data output ( light 7 ) ---------- Q7   7 ┿        ┿ 10   NOT_MR ---│---- (10)          Master reset ( active LOW )
                                               ┌--- GND   8 ┿        ┿ 9    Q7' ---┐  │                   Serial data output
                                               │            └--------┘             │  │
                                               │            ┌--------┐             │  │
         Parallel data output ( light 9 ) -----│---- Q1   1 ┿ 〇     ┿ 16   VCC ---│--┘
         Parallel data output ( light a ) -----│---- Q2   2 ┿        ┿ 15   Q0 ----│------- ( light 8 )   Parallel data output
         Parallel data output ( light b ) -----│---- Q3   3 ┿        ┿ 14   DS ----┘                      Serial data input
         Parallel data output ( light c ) -----│---- Q4   4 ┿  74HC  ┿ 13   NOT_OE -------- (9)           Output enable ( active LOW )
         Parallel data output ( light d ) -----│---- Q5   5 ┿  595   ┿ 12   ST_CP --------- (11)          Storage register clock pulse
         Parallel data output ( light e ) -----│---- Q6   6 ┿        ┿ 11   SH_CP --------- (12)          Shift register Clock pulse
         Parallel data output ( light f ) -----│---- Q7   7 ┿        ┿ 10   NOT_MR -------- (10)          Master reset ( active LOW )
                                               ├--- GND   8 ┿        ┿ 9    Q7'                           Serial data output
                                               │            └--------┘
                                               │
                                               │
    -------------------------------------------┴------------------------------------------------------------------------------------- 0 V
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
