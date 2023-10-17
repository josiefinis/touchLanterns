#include "Lanterns.h"
#include "Arduino.h"

#define MONITOR_ON              false
#define LIMITED_MONITOR_ON      false
#define MONITOR_BUTTON_PRESS    true

#define SHORT_PRESS             0b10U
#define LONG_PRESS              0b01U
#define MEAN_CANDLE_LIFE        63     //   / 4 minutes  OBS! max 63 aka 0x3F aka 0b0011 1111
#define RANGE_CANDLE_LIFE       8     //   / 4 minutes


Queue::Queue() {
  pHead = nullptr;
}


bool Queue::isEmpty() {
// True if queue has no nodes.
  if ( pHead == nullptr ) {
    return true;
  }
  else {
    return false;
  }
}


void Queue::enqueue(Candle* candle) {
// Put candle at back of queue.
  node* newNode = new node;
  newNode->pCandle = candle;
  newNode->pNext = nullptr;

  if ( isEmpty() ) { 
    pHead = newNode;
  }
  else {
    pBack->pNext = newNode;
  }
  pBack = newNode;
}


Candle* Queue::dequeue() {
// Take candle from front of queue.
  Candle* candle = pHead->pCandle;
  node* oldHead = pHead;
  pHead = oldHead->pNext;
  delete oldHead;

  return candle;
}


#if MONITOR_ON
void Queue::print() {
// Print the queue.
  Serial.print("Queue: ");
  node* q = pHead;
  while ( q ) { Serial.print((long) q->pCandle); Serial.print("->>"); q = q->pNext; }
  Serial.print("\n");
}
#endif


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
  if ( 0x55555555UL & input ) {   
    Candle* origin = &pCandleArray[longPressIndex(input)];
    buildBeaconTree(origin);
  }

  for ( uint8_t i=0; i<16; i++ ) {
    if ( input >> 2*i & LONG_PRESS ) {
      pCandleArray[i].toggleIsLit();
      pCandleArray[i].toggleIsLitOnCount(0);
      activeCounters++;
      #if MONITOR_BUTTON_PRESS
      Serial.print("\tLONG PRESS "); Serial.print(i);
      #endif
    }
    if ( input >> 2*i & SHORT_PRESS ) { 
      pCandleArray[i].toggleIsLitOnCount(0);
      activeCounters++;
      #if MONITOR_BUTTON_PRESS
      Serial.print("\tSHORT PRESS "); Serial.print(i);
      #endif
    }
  }
}


void Lanterns::burnDown() { 
// Increment counter for all lit candles 
  for ( uint8_t i = random(4); i < 16; i+=4 ) {
    if ( pCandleArray[i].getState() == 0xFFU ) { 
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


void Lanterns::buildBeaconTree(Candle* origin) {
// Build a tree of candles that watch their parent node and follow suit when that candle toggles on or off. Starts with this candle.
  #if MONITOR_ON
  for ( uint8_t i=0; i<16; i++ ) {
    Serial.print(i, HEX); Serial.print(": "); Serial.println((long) &pCandleArray[i]);
  }
  #endif

  origin->setWatching(nullptr);
  Queue queue;
  queue.enqueue(origin);
  uint16_t neighbourhood;
  uint16_t neighbour;
  Candle* newBeacon = nullptr;

  while ( not queue.isEmpty() ) {
    newBeacon = queue.dequeue();
    neighbourhood = newBeacon->getNeighbours();
    uint8_t numberOfWatchers = 0;
    for ( uint8_t i=0; i<4; i++ ) {
      neighbour = neighbourhood >> 4*i & 0xF;
      Candle* pNeighbour = &pCandleArray[neighbour];
      if ( pNeighbour == newBeacon ) { continue; } // A candle lists itself as a neighbour to represent an empty neighbour slot.
      if ( pNeighbour == origin ) { continue; }
      if ( pNeighbour->isWatching() ) { continue; }
      
      pNeighbour->setWatching(newBeacon);
      numberOfWatchers++;
      queue.enqueue(pNeighbour);
      if ( numberOfWatchers > 1 ) { break; }
    }

    #if MONITOR_ON
    Serial.print("New beacon: "); Serial.println((long) newBeacon);
    Serial.print("Neighbourhood: "); Serial.println(neighbourhood, HEX);
    queue.print();
    #endif
  }
}
