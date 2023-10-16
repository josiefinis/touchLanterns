#include "Candle.h"
#include "Arduino.h"

#define BEACON_MIN_DELAY  3     //   / 200 ms
#define BEACON_MAX_DELAY  8     //   / 200 ms


const uint8_t Candle::SHUFFLE[6] = {0x1B, 0x27, 0x4B, 0x63, 0x87, 0x93};

Candle::Candle() {
  state = 0x40;
  watching = nullptr;
}


uint8_t Candle::cut(uint8_t sequence) {
  uint8_t i = random(4);
  return (sequence << 2*i) + (sequence >> 8 - 2*i);
}


uint8_t Candle::getState() {
  return state;
}


uint16_t Candle::getNeighbours() {
// Return this candle's four neighbours as four 4-bit indices, in random order.
  uint16_t shuffledNeighbours = 0;
  uint8_t randomOrder = cut(SHUFFLE[random(6)]);
  uint8_t neighbour;
  uint8_t order;
  Serial.print("neighbours "); Serial.println(neighbours, HEX);
  for ( uint8_t i=0; i<16; i++ ) {
    Serial.print("random order "); Serial.println(randomOrder, BIN);
    Serial.print("shuffled neighbours "); Serial.println(shuffledNeighbours, HEX);
    neighbour = (neighbours >> 4*i & 0xF);
    order = (randomOrder >> 2*i & 0x3);
    shuffledNeighbours |= (neighbours >> 4*i & 0xF) << 4*(randomOrder >> 2*i & 0x3);
  }
  return shuffledNeighbours;
}


Candle* Candle::getWatching() {
  return watching;
}


bool Candle::isWatching() {
  if ( watching ) { return true; }
  return false;
}


bool Candle::isLit() {
// True if candle is lit.
  return state & 0b10000000;
}


bool Candle::isCounting() {
// True if counter is running (counting up).
  return state & 0b00100000;
}


bool Candle::isChangedLastCycle() {
// True when candle changes to a new state (lit <-> unlit).
  return not (state & 0b01000000);
}


bool Candle::isSignaled() {
// True if the candle that this candle is watching changes state.
  if ( not watching ) { return false; }
  if ( not watching->isChangedLastCycle() ) { return false; }
  return true;
}


void Candle::incrementCounter() {
  state++;
}

  
void Candle::toggleIsLit() {
// Set 
  state |= 0b01111111;
}
  

void Candle::setNeighbours(uint16_t fourNeighbours) {
// Set neighbouring candles by index (four 4-bit indices).
  neighbours = fourNeighbours;
}


void Candle::setWatching(Candle* pCandle) {
// Set pointer to candle that this candle is watching.
  watching = pCandle;
}


void Candle::setCounterRemaining(uint8_t value) {
// Set counter such that it maxes out in 'value' counts.
  state &= 0xE0;
  state |= 0x1F & ~value;
}


void Candle::setNotChangedLastCycle() {
// Reset changedLastCycle flag to .
  state |= 0b01000000;
}


void Candle::setLifeRemaining(uint8_t value) {
// Set life counter such that it maxes out in 'value' counts.
// NB The life counter hijacks the isCounting bit (6 bit)
  state &= 0xC0;
  state |= 0x3F & ~value;
}


void Candle::followSuit() {
  if ( not watching ) { return false; }
  state |= 0b01111111;
  if ( isLit() == watching->isLit() ) {
    state ^= 0b10000000; 
    setCounterRemaining(0);
  }
  else {
    setCounterRemaining(random(BEACON_MIN_DELAY, BEACON_MAX_DELAY));
  }
  watching = nullptr;
}


void Candle::burnDown() {
  if ( isLit() ) { state++; }
}
