// SIPO Shift Register. 
#ifndef REGISTER_H
#define REGISTER_H
#include "Arduino.h"


class Register {
  public:
    Register();

    void writeEvery2ndBit(uint32_t bits);
    void storeShiftRegister();
    void writeToStorageRegister(uint16_t word);
    void reset();

  private:
    void writeToShiftRegister(uint16_t word);
    void writeBit(bool bit);
    void clearShiftRegister();
    void enableOutput();
    void disableOutput();
    void shiftRegisterClockPulse();
    void storageRegisterClockPulse();

};
#endif
