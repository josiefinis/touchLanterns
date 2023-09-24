// SIPO Shift Register. 
#ifndef REGISTER_H
#define REGISTER_H
#include "Arduino.h"
#endif

#define HARDWARE_REGISTER_SIZE 16
#define PIN_REGISTER_SER 9
#define PIN_REGISTER_NOT_OE 6
#define PIN_REGISTER_RCLK 11
#define PIN_REGISTER_SRCLK 12
#define PIN_REGISTER_NOT_SRCLR 7

class Register {
  private:

    void writeToShiftRegister(uint16_t word);
    void clearShiftRegister();
    void enableOutput();
    void disableOutput();
    void shiftRegisterClockPulse();
    void storageRegisterClockPulse();

  public:
    Register();

    void writeBit(bool bit);
    void writeToStorageRegister(uint16_t word);
    void storeShiftRegister();
};
