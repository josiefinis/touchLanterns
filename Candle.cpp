#include "Candle.h"
#include "Arduino.h"

#define ANY_PRESS  0b10
#define LONG_PRESS 0b01

#define BEACONS_ON true


Candle::Candle() {
  state = 0x40;
  watching = nullptr;
  next = nullptr;
}

 
uint16_t  Candle::litCandles = 0;          
uint8_t  Candle::activeCounters = 0;
bool Candle::newChanges = false;


void Candle::receiveSignal(Candle candleArray[16], uint32_t input) {
  uint8_t i = 0;
  while ( input ) {
    if ( input & ANY_PRESS ) {
      candleArray[i].toggleIsLit();
      activeCounters++;
    }
    if ( input & LONG_PRESS ) { candleArray[i].buildBeaconNetwork(candleArray, i); }
    input >>= 2;
    i++;
  }
}


void Candle::update(uint8_t i) {
  // Handle timer interrupt events. 
  if ( activeCounters == 0 ) { return; }
  if ( isWatching() and watching->changedLastCycle() ) { followSuit(); }
  if ( changedLastCycle() ) { 
    litCandles ^= indexToOneHot(i);
    newChanges = true;
    setNotChangedLastCycle(); 
    activeCounters--;
    // TODO rework state: don't store lit/unlit, store that in static variable.
  } 
  if ( isCounting() ) { state++; }
}


bool Candle::hasUpdatesForRegister() { return newChanges; } 
uint16_t Candle::getLitCandles() { return litCandles; } 
uint8_t Candle::getActiveCounters() { return activeCounters; }


uint16_t Candle::indexToOneHot(uint8_t idx) {
// Return 1-hot encoding of index.
  return 0x0001 << idx;
}


Candle* Candle::getWatching() {
  return watching;
}


bool Candle::isWatching() {
  if ( watching ) { return true; }
  return false;
}


void Candle::followSuit() {
  Serial.print("followSuit ");
  if ( not watching ) { return false; }
  state |= 0b01111111;
  if ( isLit() == watching->isLit() ) {
    state ^= 0b10000000; 
    setRemainingCounter(4);
  }
  else {
    state |= 0b01100000; //TODO delete line
    setRemainingCounter(random(4,12));
  }
  watching = nullptr;
}


#if BEACONS_ON
void Candle::buildBeaconNetwork(Candle candleArray[16], uint8_t idx) {
// Build tree of candles watching others to follow suit, starting with candle at idx.
  Serial.print("buildBeaconNetwork ");
  watching = this;
  Candle* queue = this;
  Candle* last = this;
  while ( queue ) {
     *last->next = *queue->findWatchBeaconAbove(candleArray, idx);
     if ( &(*last->next) ) { *last = *last->next; }
     *last->next = *queue->findWatchBeaconBelow(candleArray, idx);
     if ( &(*last->next) ) { *last = *last->next; }
     *queue = *queue->next;
  } 
}


Candle* Candle::findWatchBeaconAbove(Candle candleArray[16], uint8_t idx) {
// Try to assign 1 watcher with idx 1 or 2 steps above this candle, if possible. 
  Serial.print("findWatchBeaconAbove ");
  uint8_t step = 0;
  step = 0x80 + random(2); step++;
  while ( step ) {
    if ( linkWatchBeacon(candleArray, idx + step) ) { return &candleArray[idx + step]; }
    step >>= 2;
  }
  return nullptr;
}


Candle* Candle::findWatchBeaconBelow(Candle candleArray[16], uint8_t idx) {
// Try to assign 1 watcher with idx 1 or 2 steps below this candle, if possible. 
  Serial.print("findWatchBeaconBelow ");
  uint8_t step = 0;
  step = 0x80 + random(2); step++;
  while ( step ) {
    if ( linkWatchBeacon(candleArray, idx - step) ) { return &candleArray[idx - step]; }
    step >>= 2;
  }
  return nullptr;
}

  
bool Candle::linkWatchBeacon(Candle candleArray[16], uint8_t idx) {
// Set candle at idx to watch this candle if not already watching a candle.
  Serial.print("linkWatchBeacon ");
  if ( idx > 15 ) { return false; }
  if ( not candleArray[idx].isWatching() ) { 
    candleArray[idx].watching = this;
    return true;
  } 
  return false;
}
#endif


uint8_t Candle::getState() {
  return state;
}


void Candle::toggleIsLit() {
// Set 
  state |= 0b01111111;
}
  

bool Candle::isLit() {
// True if candle is lit.
  return state & 0b10000000;
}


bool Candle::changedLastCycle() {
// True when candle changes to a new state (lit <-> unlit).
  return not (state & 0b01000000);
}


void Candle::setNotChangedLastCycle() {
// Reset changedLastCycle flag to .
  state |= 0b01000000;
}


bool Candle::isCounting() {
// True if counter is running (counting up).
  return state & 0b00100000;
}


void Candle::setRemainingCounter(uint8_t value) {
// Set remaining counter to value.
  state &= 0xE0;
  state |= 0x1F & ~value;
}

void Candle::burn() { 
// 
  state--; 
}
