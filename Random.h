/*
======================================================================================================================================================
                            RANDOM
======================================================================================================================================================
*/

#ifndef RANDOM_H
#define RANDOM_H

#include "Arduino.h"

class Random {
  public:
    Random();
    static uint8_t uRandN(uint8_t n);
    static uint8_t urandom(uint8_t n);
    static void seed(uint8_t bits);
    static void push(uint8_t bits);
    static uint8_t pull(uint8_t n);

  private:
    static uint8_t stackSize;
    static uint32_t stack;
    static uint32_t pseudoRandom;
    static void newPseudoRandom();
    static void fillStack();
}; 
#endif
