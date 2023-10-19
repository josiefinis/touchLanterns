#include "Candle.h"

#define BEACON_MIN_DELAY      3     //   / 200 ms
#define BEACON_MAX_DELAY      8     //   / 200 ms


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


uint8_t Candle::getLifeRemaining() {
  return 0x1F - (state & 0x1F);
}


uint16_t Candle::getNeighbours() {
// Return this candle's four neighbours as four 4-bit indices, in random order.
  uint16_t shuffledNeighbours = 0;
  uint8_t randomOrder = cut(SHUFFLE[random(6)]);
  uint8_t neighbour;
  uint8_t order;
  for ( uint8_t i=0; i<4; i++ ) {
    neighbour = (neighbours >> 4*i & 0xF);
    order = (randomOrder >> 2*i & 0x3);
    shuffledNeighbours |= neighbour << 4*order;
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

  
void Candle::toggleIsLitOnCount(uint8_t count) {
// Set candle to change from lit to unlit or vice versa next cycle.
  state |= 0b01111111;
  setCounterRemaining(count);
}


void Candle::toggleIsLit() {
// Change from lit to unlit or vice versa.
  state ^= 0b10000000;
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
  state &= 0xE0U;
  state |= 0x1FU & ~value;
}


void Candle::setNotChangedLastCycle() {
// Reset changedLastCycle flag to .
  state |= 0b01000000;
}


void Candle::setLifeRemaining(uint8_t value) {
  state &= 0xC0U;
  state |= 0x1FU & ~value;
}


void Candle::followSuit() {
  if ( not watching ) { return false; }
  if ( isLit() == watching->isLit() ) {
    toggleIsLit(); // flip the state bit briefly so that it will flip back and match the candle it is watching.
    toggleIsLitOnCount(0);
  }
  else {
    toggleIsLitOnCount(random(BEACON_MIN_DELAY, BEACON_MAX_DELAY));
  }
  watching = nullptr;
}


void Candle::burnDown() {
  if ( isLit() ) { state++; }
}
