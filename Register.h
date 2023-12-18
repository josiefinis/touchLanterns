/*
======================================================================================================================================================
                                REGISTER
======================================================================================================================================================

Writes to hardware SIPO shift register.
*/

#ifndef REGISTER_H
#define REGISTER_H
#include "Arduino.h"


class Register {
  public:
    Register();

    void reset( void );
    void writeToStorageRegister( uint16_t word );
    void storeShiftRegister();

  private:
    void writeToShiftRegister( uint16_t word );
    void writeBit( bool bit );
    void clearShiftRegister( void );
    void enableOutput( void );
    void disableOutput( void );
    void shiftRegisterClockPulse( void );
    void storageRegisterClockPulse( void );

};
#endif
