#include "Global.h"
#include "PWMSignal.h"
#include "Arduino.h"


PWMSignal::PWMSignal() {
  // Make zeroth node with signal = 0 and time = 0
  pZero = newEdge(0, 0);
  nextSignal = 0;
}


void PWMSignal::changeDuty(uint8_t index, uint8_t brightness) {
  // Change the PWM signal to reflect the new brightness for lantern with index 'index'.
  uint16_t signal = 1 << index;
  removeEdge(signal);
  if ( brightness == 0xFF ) {
    pZero->signal |= signal;
    return 0;
  }
  if ( brightness == 0 ) {
    pZero->signal &= ~signal;
    return 0;
  }

  uint16_t pulseWidth = pulseWidthQuadratic(brightness);
  if ( pulseWidth > PWM_PERIOD / 2 ) {
    pZero->signal |= signal;
    insertEdge(signal, pulseWidth);
  }
  else {
    pZero->signal &= ~signal;
    insertEdge(signal, PWM_PERIOD - pulseWidth);
  }
  //Serial.print(brightness); Serial.print("\t"); Serial.print(pulseWidth); Serial.print("\n");
}


uint16_t PWMSignal::getSignal() {
// Return the next signal to write to the register.
  return nextSignal;
}


uint16_t PWMSignal::getTime() {
// Return the time when the next signal is due (in microseconds after the start of the PWM period).
  return nextTime;
}


void PWMSignal::nextEdge() {
// Cue up the next pulse edge
  if ( pEdge->pNext ) {
    pEdge = pEdge->pNext;
    nextSignal ^= pEdge->signal;
    nextTime = pEdge->time;
    if ( nextTime == 0 ) { 
      nextTime = 0xFFFF; 
    }
  }
  else {
    nextSignal = pZero->signal;
    nextTime = 0xFFFF;
  }
}


void PWMSignal::periodStart() {
  pEdge = pZero;
}


#if MONITOR_PWM_LIST
void PWMSignal::printSignalList() {
  pulseEdge* pNode = pZero;
  while ( pNode ) {
    Serial.print("0b"); Serial.print(pNode->signal, BIN); Serial.print(":"); Serial.print(pNode->time); Serial.print("  ->  ");
    pNode = pNode->pNext;
  }
}
#endif


void PWMSignal::insertEdge(uint16_t signal, uint16_t time) {
// Insert a pulse edge in the list in chronological order.
  if ( not time ) { 
    return 0;
  }
  pulseEdge* pNode = pZero;
  while ( pNode->pNext ) {
    if ( pNode->pNext->time == time ) {  
      pNode->pNext->signal |= signal;
      return 0;
    }
    if ( pNode->pNext->time > time ) { 
      pulseEdge* pNew = newEdge(signal, time);
      pNew->pNext = pNode->pNext;
      pNode->pNext = pNew;
      return 0;
    }
    pNode = pNode->pNext;
  }
  pNode->pNext = newEdge(signal, time);
}


void PWMSignal::removeEdge(uint16_t signal) {
// Remove a pulse edge signal from list.
  pulseEdge* pNode = pZero;
  while ( pNode = pNode->pNext ) {
    if ( pNode->signal & signal ) { 
      pNode->signal &= ~signal;
    }
    if ( not pNode->signal ) {
      deleteEdge(pNode);
    }
  }
}


pulseEdge* PWMSignal::newEdge(uint16_t signal, uint16_t time) { 
// Create a new pulse edge node.
  pulseEdge* pNode = new pulseEdge;
  pNode->signal = signal;
  pNode->time = time;
  pNode->pNext = nullptr;
  return pNode;
}


void PWMSignal::deleteEdge(pulseEdge* pDeadEdge) {
// Delete node when there is no longer an edge signal on it.
  pulseEdge* pNode = pZero;
  while ( pNode->pNext ) {
    if ( pNode->pNext == pDeadEdge ) {
      pNode->pNext = pDeadEdge->pNext;
      delete pDeadEdge;
      return 0;
    }
    pNode = pNode->pNext;
  }
}


// There is a choice of 4 functions that map brightness to pulse width such that
//                 f(1)  >  640 µs,         which gives lowest DC voltage that produces light from the LED candles,
//        f(x+1) - f(x) >=    4 µs,         so that f(x+1) and f(x) are distinct, 4 µs being the resolution of micros(),
//               f(256) == 9856 µs,         which is the PWM period.
// These are a linear, a quadratic, a cubic and a quartic function. 

uint16_t PWMSignal::pulseWidthLinear(uint8_t brightness) {
// Map brightness value to pulse width using linear function. 
  return 640 + 36 * brightness;
}


uint16_t PWMSignal::pulseWidthQuadratic(uint16_t brightness) {
// Map brightness value to pulse width usisng quadratic function f(x) = 4x + 2*(x/4)^2.
  return 640 + 4 * brightness + brightness / 2 * brightness / 4;
}


uint16_t PWMSignal::pulseWidthCubic(uint16_t brightness) {
// Map brightness value to pulse width usisng cubic function f(x) = 4x + 2*(x/16)^3.
  return 640 + 4 * brightness + brightness / 2 * brightness / 256 * brightness / 8;
}


uint16_t PWMSignal::pulseWidthQuartic(uint32_t brightness) {
// Map brightness value to pulse width using quartic function f(x) = 4x + 2*(x/32)^4.
  return 640 + 4 * brightness + brightness / 2 * brightness / 256 * brightness / 256 * brightness / 8;
}


