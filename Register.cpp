/*
======================================================================================================================================================
                                REGISTER
======================================================================================================================================================
*/
#include "Register.h"
#include "Global.h"
#include "Arduino.h"


#define HARDWARE_REGISTER_SIZE 16
Register::Register() 
{
  disableOutput();
}


void Register::reset( void ) {
// Clear everything.
  disableOutput();
  clearShiftRegister();
  storeShiftRegister();
  enableOutput();
}


void Register::writeToStorageRegister( uint16_t word ) {
// Feed a word into the shift register and copy to the storage register.
  writeToShiftRegister(word);
  storeShiftRegister();
}


void Register::writeToShiftRegister( uint16_t word ) {
// Feed word into shift register, least significant bit first.
  for ( uint8_t i = 0; i < HARDWARE_REGISTER_SIZE; i++ ) {
    bool bit = word & 1;
    writeBit( bit );
    word >>= 1;
  }
}


void Register::writeBit( bool bit ) {
// Write a single bit to the shift register, shifting previously written bits down one place.
  if ( bit ) { 
    PORTB |= PIN_REGISTER_SER; 
  }
  else { 
    PORTB &= ~PIN_REGISTER_SER; 
  }
  shiftRegisterClockPulse();
}


void Register::storeShiftRegister( void ) { 
  storageRegisterClockPulse(); 
}


void Register::enableOutput( void ) { 
  PORTB &= ~PIN_REGISTER_NOT_OE; 
}


void Register::disableOutput( void ) { 
  PORTB |= PIN_REGISTER_NOT_OE;
}


void Register::clearShiftRegister( void ) { 
  PORTB &= ~PIN_REGISTER_NOT_SRCLR;
  PORTB |= PIN_REGISTER_NOT_SRCLR;
}


void Register::shiftRegisterClockPulse( void ) {
  PORTB |= PIN_REGISTER_SRCLK;
  PORTB &= ~PIN_REGISTER_SRCLK;
}


void Register::storageRegisterClockPulse( void ) {
  PORTB |= PIN_REGISTER_RCLK;
  PORTB &= ~PIN_REGISTER_RCLK;
}
