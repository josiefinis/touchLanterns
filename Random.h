// Random number generator.
#ifndef RANDOM_H
#define RANDOM_H

#include "Arduino.h"

class Random {
  public:
    Random::Random();
    static uint8_t urandom(uint16_t n);
    static void seed(uint8_t bits);
    static void push(uint8_t bits);
    static uint8_t pull(uint16_t n);

  private:
    static uint8_t stackSize;
    static uint32_t stack;
    static uint32_t pseudoRandom;
    static void newPseudoRandom();
    static void fillStack();
}; 
#endif
