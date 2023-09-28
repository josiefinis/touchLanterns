#include "Register.h"
#include "Arduino.h"


#define HARDWARE_REGISTER_SIZE 16
Register::Register() 
{
  disableOutput();
}


void Register::reset() {
// Clear everything.
  disableOutput();
  clearShiftRegister();
  storeShiftRegister();
  enableOutput();
}


void Register::writeToStorageRegister(uint16_t word) {
// Feed a word into the shift register and copy to the storage register.
  writeToShiftRegister(word);
  storeShiftRegister();
}


void Register::writeToShiftRegister(uint16_t word)
// Feed word into shift register, least significant bit first.
{
  for ( uint8_t i = 0; i < HARDWARE_REGISTER_SIZE; i++ ) {
    bool bit = word & 1;
    writeBit(bit);
    word >>= 1;
  }
}


void Register::writeBit(bool bit)
// Write a single bit to the shift register, shifting previously written bits down one place.
{
  if (bit) { digitalWrite(PIN_REGISTER_SER, HIGH); }
  else { digitalWrite(PIN_REGISTER_SER, LOW); }
  shiftRegisterClockPulse();
}


void Register::storeShiftRegister() { storageRegisterClockPulse(); }
void Register::enableOutput() { digitalWrite(PIN_REGISTER_NOT_OE, LOW); }
void Register::disableOutput() { digitalWrite(PIN_REGISTER_NOT_OE, HIGH); }


void Register::clearShiftRegister() 
{ 
  digitalWrite(PIN_REGISTER_NOT_SRCLR, LOW);
  digitalWrite(PIN_REGISTER_NOT_SRCLR, HIGH);
}


void Register::shiftRegisterClockPulse()
{
  digitalWrite(PIN_REGISTER_SRCLK, HIGH);
  digitalWrite(PIN_REGISTER_SRCLK, LOW);
}


void Register::storageRegisterClockPulse()
{
  digitalWrite(PIN_REGISTER_RCLK, HIGH);
  digitalWrite(PIN_REGISTER_RCLK, LOW);
}
