#include "Candle.h"
#include "Arduino.h"

#define ANY_PRESS  0b10
#define LONG_PRESS 0b01

#define BEACONS_ON true
#define MONITOR_ON true


Candle::Candle() {
  state = 0x40;
  watching = nullptr;
  next = nullptr;
}

 
uint8_t Candle::zeroAddress = 0;
uint8_t Candle::addressStep = 0;
bool Candle::busy = false;
uint16_t  Candle::litCandles = 0;          
uint8_t  Candle::activeCounters = 0;
bool Candle::newChanges = false;


void Candle::storeAddress(Candle candleArray[16]) {
// Store memory address of candle array and size of step between array elements.
  zeroAddress = (uint8_t) &candleArray[0];
  addressStep = (uint8_t) &candleArray[1] - zeroAddress;
}


uint8_t Candle::addressToIndex(uint8_t address) {
// Return candleArray index of candle at address.
  return (address - zeroAddress) / addressStep;
}


void Candle::receiveSignal(Candle candleArray[16], uint32_t input) {
// Handle input from buttons.
  busy = true;
  #if MONITOR_ON
  Serial.print("input: "); Serial.print(input, HEX); Serial.print("\n");
  #endif
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
  busy = false;
}


void Candle::periodicUpdate(uint8_t i) {   
  // Handle timer interrupt events. 
  if ( activeCounters == 0 ) { return; }
  busy = true;
  if ( isWatching() and watching->changedLastCycle() ) { followSuit(); activeCounters++; }
  if ( changedLastCycle() ) { 
    litCandles ^= indexToOneHot(i);
    newChanges = true;
    if ( isLit() ) { setCounterRemaining(64 - random(8)); }
    setNotChangedLastCycle(); 
    activeCounters--;
    // TODO rework state: don't store lit/unlit, store that in static variable.
  } 
  if ( isCounting() ) { state++; }
  busy = false;
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
  if ( not watching ) { return false; }
  state |= 0b01111111;
  if ( isLit() == watching->isLit() ) {
    state ^= 0b10000000; 
    setCounterRemaining(4);
  }
  else {
    // state |= 0b01100000; //TODO delete line
    setCounterRemaining(random(4,12));
  }
  watching = nullptr;
}


#if BEACONS_ON
void Candle::buildBeaconNetwork(Candle candleArray[16], uint8_t idx) {
// Build tree of candles watching others to follow suit, starting with candle at idx.
  watching = nullptr;
  Candle* head = this; 
  #if MONITOR_ON
  Serial.print("buildBeaconNetwork, starting from ");
  Serial.print((long) this, HEX); Serial.print(".\n"); 
  #endif
  Candle* last = this;
  while ( head ) {
     last->next = findWatchBeaconAbove(candleArray, addressToIndex((uint8_t) head));   
     if ( (last->next) ) { last = last->next; }                  

     last->next = findWatchBeaconBelow(candleArray, addressToIndex((uint8_t) head));   
     if ( (last->next) ) { last = last->next; }                 
    
     Candle* done = head;
     head = head->next;                                           // move head to next in queue.
     done->next = nullptr;

     #if MONITOR_ON
     Serial.print("Queue: ");
     Candle* q = head;
     while ( q ) { Serial.print((long) q); Serial.print("->>"); q = q->next; }
     Serial.print("\n");
     #endif
  } 
}


Candle* Candle::findWatchBeaconAbove(Candle candleArray[16], uint8_t idx) {
// Try to assign 1 watcher with idx 1 or 2 steps above this candle, if possible. 
  uint8_t step = 0b0110 + 3*random(2);
  //step = 0x80 + random(2); step++;
  while ( step ) {
    if ( linkWatchBeacon(candleArray, idx + (0b11 & step), idx) ) { return &candleArray[idx + (0b11 & step)]; }
    step >>= 2;
  }
  return nullptr;
}


Candle* Candle::findWatchBeaconBelow(Candle candleArray[16], uint8_t idx) {
// Try to assign 1 watcher with idx 1 or 2 steps below this candle, if possible. 
  uint8_t step = 0b0110 + 3*random(2);
  //step = 0x80 + random(2); step++;    //1000 0001 or 1000 0010    ummm oops, want either 0110 or 1001 i.e. 6 or 9 
  while ( step ) {
    if ( linkWatchBeacon(candleArray, idx - (0b11 & step), idx) ) { return &candleArray[idx - (0b11 & step)]; }
    step >>= 2;
  }
  return nullptr;
}

  
bool Candle::linkWatchBeacon(Candle candleArray[16], uint8_t watcher, uint8_t watched) {
// Set candle at idx to watch this candle if not already watching a candle.
  if ( watcher > 15 ) { return false; }
  if ( candleArray[watcher].isWatching() ) { return false; }
  if ( this == &candleArray[watcher] ) { return false; }
  candleArray[watcher].watching = &candleArray[watched];
  #if MONITOR_ON
  Serial.print(watcher); Serial.print("->"); Serial.print(watched); Serial.print("\n");
  #endif
  return true;
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


void Candle::setCounterRemaining(uint8_t value) {
// Set counter such that it maxes out in value counts.
  state &= 0xE0;
  state |= 0x1F & ~value;
}


void Candle::burnDown(Candle candleArray[16], uint16_t litCandles) { 
// Increment counter for all lit candles 
  uint8_t i = 0;
  while ( litCandles ) {
    if ( not litCandles & 1 ) { continue; }
    if ( candleArray[i].isCounting() ) { continue; }
    candleArray[i].state++; 
    i++;
    litCandles >>= 1;
  }
}
