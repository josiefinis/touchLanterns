#include "Candle.h"
#include "Arduino.h"

#define ANY_PRESS  0b10
#define LONG_PRESS 0b01


Candle::Candle() {
  state = 0x40;
  watching = nullptr;
  next = nullptr;
}

 
void Candle::receiveSignal(uint8_t i, uint8_t input) {
  // Handle input from buttons.
  Serial.println();
  Serial.print("received: "); Serial.print(i); Serial.print(" "); Serial.print(input, BIN);
  if ( input == 0 ) { return; }
  Serial.print("receiveInput ");
  if ( input & ANY_PRESS ) { 
    Serial.print("Cidx: ");Serial.print(i);Serial.print("\t");
    toggleIsLit();
    activeCounters |= indexToOneHot(i); // TODO change active counters to simple counter that keeps track of how many counters are counting.
  }
  // if ( input & LONG_PRESS ) { buildBeaconNetwork(i); }
}


uint16_t Candle::updateCycle(uint8_t i) {
  // Handle timer interrupt events. 
  uint16_t returnValue = 0;
  if ( activeCounters == 0 ) { return 0; }
  //if ( watching->changedLastCycle() ) { followSuit(); }
  if ( changedLastCycle() ) { 
    setNotChangedLastCycle(); 
    returnValue = indexToOneHot(i); 
    newChanges |= indexToOneHot(i); 
    activeCounters &= ~indexToOneHot(i);
  } 
  if ( isCounting() ) { state++; }
  return returnValue;
}


// bool Candle::update() {
//   // 
//   Serial.print("update ");
//   //TODO check finished
//   if ( watching->changedLastCycle() ) { followSuit(); }
//   if ( changedLastCycle() ) { return true; }
//   if ( isCounting() ) { state++; }
//   return false;
// }


void Candle::printCounters() { Serial.print("counters: "); Serial.print(activeCounters, BIN); Serial.print("\t"); }
void Candle::printNewChanges() { Serial.print("new changes: "); Serial.print(newChanges, BIN); Serial.print("\t"); }
void Candle::printLitCandles() { Serial.print("lit candles: "); Serial.print(litCandles, BIN); Serial.print("\t"); }


bool Candle::hasNewChanges() { return newChanges > 0; } // Require hardware register update. 
uint16_t Candle::getLitCandles() { return litCandles; } 


void Candle::applyNewChanges() { 
  Serial.print("applyNewChanges ");
  litCandles ^= newChanges; 
  newChanges = 0;
}


uint16_t Candle::indexToOneHot(uint8_t idx) {
// Return 1-hot encoding of index.
  return 0x0001 << idx;
}


Candle* Candle::getWatching() {
  return watching;
}


bool Candle::isWatching() {
  if ( watching ) { return true; }
}


#if false
void Candle::followSuit() {
  Serial.print("followSuit ");
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


void Candle::buildBeaconNetwork(uint8_t idx) {
// Build tree of candles watching others to follow suit, starting with candle at idx.
  Serial.print("buildBeaconNetwork ");
  watching = this;
  Candle* queue = this;
  Candle* last = this;
  while ( queue ) {
     *last->next = *queue->findWatchBeaconAbove(idx);
     if ( &(*last->next) ) { *last = *last->next; }
     *last->next = *queue->findWatchBeaconBelow(idx);
     if ( &(*last->next) ) { *last = *last->next; }
     *queue = *queue->next;
  } 
}


Candle* Candle::findWatchBeaconAbove(uint8_t idx) {
// Try to assign 1 watcher with idx 1 or 2 steps above this candle, if possible. 
  Serial.print("findWatchBeaconAbove ");
  uint8_t step = 0;
  step = 0x80 + random(2); step++;
  while ( step ) {
    if ( linkWatchBeacon(idx + step) ) { return candleArray[idx + step]; }
    step >>= 2;
  }
  return nullptr;
}


Candle* Candle::findWatchBeaconBelow(uint8_t idx) {
// Try to assign 1 watcher with idx 1 or 2 steps below this candle, if possible. 
  Serial.print("findWatchBeaconBelow ");
  uint8_t step = 0;
  step = 0x80 + random(2); step++;
  while ( step ) {
    if ( linkWatchBeacon(idx - step) ) { return candleArray[idx - step]; }
    step >>= 2;
  }
  return nullptr;
}

  
bool Candle::linkWatchBeacon(uint8_t idx) {
// Set candle at idx to watch this candle if not already watching a candle.
  Serial.print("linkWatchBeacon ");
  if ( idx > 15 ) { return false; }
  if ( not candleArray[idx]->watching ) { 
    candleArray[idx]->watching = this;
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
  Serial.print("toggleIsLit ");
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
  Serial.print("setNotChangedLastCycle ");
  state |= 0b01000000;
}


bool Candle::isCounting() {
// True if counter is running (counting up).
  return state & 0b00100000;
}


void Candle::setRemainingCounter(uint8_t value) {
// Set remaining counter to value.
  Serial.print("setRemainingCounter ");
  state &= 0xE0;
  state |= 0x1F & ~value;
}

void Candle::burn() { 
// 
  Serial.print("burn ");
  state--; 
}



