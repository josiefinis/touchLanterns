#include "Lanterns.h"
#include "Arduino.h"

#define MONITOR_ON false
#define LIMITED_MONITOR_ON false

#define ANY_PRESS  0b10
#define LONG_PRESS 0b01
#define MEAN_CANDLE_LIFE 63     //   / 4 minutes  OBS! max 63 aka 0x3F aka 0b0011 1111
#define RANGE_CANDLE_LIFE 8     //   / 4 minutes




Lanterns::Lanterns(Candle* pArray) {
  pCandleArray = pArray; 
  litCandles = 0;
  activeCounters = 0;
  isUpdateForRegister = false;
}


uint16_t Lanterns::getLitCandles() { return litCandles; }
bool Lanterns::getIsUpdateForRegister() { return isUpdateForRegister; } 
uint8_t Lanterns::getActiveCounters() { return activeCounters; }
void Lanterns::setIsUpdateForRegister(bool value) { isUpdateForRegister = value; }


void Lanterns::update() {   
  // Handle timer interrupt events. 
  if ( activeCounters == 0 ) { return; }
  Candle* candle = nullptr;
  for (uint8_t i=0; i<16; i++ ) {
    candle = pCandleArray + i;
    if ( candle->isSignaled() ) { 
      candle->followSuit(); 
      activeCounters++; 
    }
    if ( candle->isChangedLastCycle() ) { 
      isUpdateForRegister = true;
      if ( candle->isLit() ) { 
        candle->setLifeRemaining(MEAN_CANDLE_LIFE - random(RANGE_CANDLE_LIFE));
        litCandles |= (1 << i);
      }
      else {
        litCandles &= ~(1 << i);
      }
      candle->setNotChangedLastCycle(); 
      activeCounters--;
    } 
    if ( candle->isCounting() ) { candle->incrementCounter(); }
  }
}


void Lanterns::receiveSignal(uint32_t input) {
// Handle input from buttons.
  // If long press, quickly build tree before next interrupt.
  if ( 0x55555555L & input ) {   
    Candle* origin = &pCandleArray[longPressIndex(input)];
    buildBeaconTree(origin);
  }

  #if MONITOR_ON
  Serial.print("input: "); Serial.print(input, HEX); Serial.print("\n");
  #endif

  uint8_t i = 0;
  while ( input ) {
    if ( input & ANY_PRESS ) {
      pCandleArray[i].toggleIsLit();
      activeCounters++;
    }
    input >>= 2;
    i++;
  }
}


void Lanterns::burnDown() { 
// Increment counter for all lit candles 
  for ( uint8_t i = random(4); i < 16; i+=4 ) {
    if ( pCandleArray[i].getState() == 0xFF ) { 
      activeCounters++; 
      return 0; 
    }
    pCandleArray[i].burnDown();
  }
}


uint8_t Lanterns::longPressIndex(uint32_t input) {
// Return index of button what was long-pressed.
  uint8_t i = 0;
  while ( input ) {
    if ( input & LONG_PRESS ) { return i; }
    i++;
    input >>= 2;
  }
}


void Lanterns::buildBeaconTree(Candle* originCandle) {
// Build a tree of candles that watch their parent node and follow suit when that candle toggles on or off. Starts with this candle.
  queueNode origin;
  origin.pCandle = originCandle;
  origin.next = nullptr;
  origin.pCandle->setWatching(nullptr);
  
  queueNode* pFirst = &origin;
  queueNode* pLast = &origin;

  while ( pFirst ) {
    uint16_t neighbours = pFirst->pCandle->getNeighbours();
    uint8_t numberOfWatchers = 0;
    while ( neighbours ) {
      Candle* pNeighbour = &pCandleArray[neighbours & 0xF];
      neighbours >>= 4;
      if ( pNeighbour == origin.pCandle ) { continue; }
      if ( pNeighbour->isWatching() ) { continue; }
      pNeighbour->setWatching(pFirst->pCandle);
      queueNode newLast;
      newLast.pCandle = pNeighbour;
      newLast.next = nullptr;
      pLast->next = &newLast;
      pLast = &newLast;
      numberOfWatchers++;
      if ( numberOfWatchers > 1 ) { break; }
    }
    pFirst = pFirst->next;

    #if LIMITED_MONITOR_ON
    Serial.print("Queue: ");
    queueNode* q = pFirst;
    while ( q ) { Serial.print(addressToIndex((uint8_t) q->item)); Serial.print("->>"); q = q->next; }
    Serial.print("\n");
    #endif
  }
}
