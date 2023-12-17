/*
======================================================================================================================================================
                            RANDOM
======================================================================================================================================================
*/

#ifndef RANDOM_CPP
#define RANDOM_CPP

#include "Random.h"


uint8_t Random::stackSize = 0;
uint32_t Random::stack = 0;
uint32_t Random::pseudoRandom = 0;


Random::Random() { } 


void Random::newPseudoRandom() {
// Set next pseudorandom number using xorshift algorithm.
  pseudoRandom ^= pseudoRandom << 13;
  pseudoRandom ^= pseudoRandom >> 17;
  pseudoRandom ^= pseudoRandom << 5;
}
 

void Random::fillStack() {
// Fill stack with pseudorandom bits.
  newPseudoRandom();
  stack = pseudoRandom;
  stackSize = 32;
}


void Random::seed(uint8_t bits) {
// Push 8 bits onto pseudoRandom.
  pseudoRandom <<= 8;
  pseudoRandom ^= bits;
}


void Random::push(uint8_t bits) {
// Push 8 bits onto stack.
  stack <<= 8;
  stack ^= bits;
  stackSize += 8;
  if ( stackSize > 32 ) { stackSize = 32; }
}


uint8_t Random::pull(uint8_t n) {
// Pull n bits from stack.
  if ( stackSize < n ) {
    fillStack();
  }
  uint8_t bits = stack & ((1 << n) - 1);
  stack >>= n;
  stackSize -= n;
  return bits;
}


uint8_t Random::urandom(uint8_t n) {
  return n * pull( 8 ) / 256;
}
#endif
