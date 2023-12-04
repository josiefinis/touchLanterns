#include "Global.h"
#include "PWMSignal.h"
#include "Arduino.h"


PWMSignal::PWMSignal() {
  // Make zeroth node with signal = 0 and time = 0
  pZero = newEdge(0, 0);
  nextSignal = 0;
  nEdges = 0;
}


void PWMSignal::reset() {
// Reset in case of crash recovery. Does not release memory.
  pZero->signal = 0;
  pZero->time = 0;
  pZero->pNext = nullptr; // Does not release memory!
  periodStart();
}


int8_t PWMSignal::changeDuty(uint8_t index, uint8_t brightness) {
  // Change the PWM signal to reflect the new brightness for lantern with index 'index'.
  uint16_t signal = 1 << index;
  if ( removeEdge(signal) == -1 ) { 
    return -2; 
  }
  if ( brightness == 0xFF ) {
    pZero->signal |= signal;
    return 0;
  }
  if ( brightness == 0 ) {
    pZero->signal &= ~signal;
    return 0;
  }

  uint16_t pulseWidth = pulseWidthQuadratic(brightness);
  if ( pulseWidth < 0 ) { 
    Serial.print( "Warning: invalid pulse width: " ); Serial.print( pulseWidth );
  } 
  if ( pulseWidth > PWM_PERIOD ) { 
    Serial.print( "Warning: invalid pulse width: " ); Serial.print( pulseWidth );
  } 
  if ( pulseWidth > PWM_PERIOD / 2 ) {
    pZero->signal |= signal;
    if ( insertEdge(signal, pulseWidth) < 0 ) {
      return -3;
    }
  }
  else {
    pZero->signal &= ~signal;
    if ( insertEdge(signal, PWM_PERIOD - pulseWidth) < 0 ) {
      return -3;
    }
  }
  if ( pZero->time ) { Serial.print( "Warning: pZero = " ); Serial.print( pZero->time ); }
}


uint16_t PWMSignal::getSignal() {
// Return the next signal to write to the register.
  return nextSignal;
}


uint16_t PWMSignal::getTime() {
// Return the time when the next signal is due (in microseconds after the start of the PWM period).
  return nextTime;
}


int8_t PWMSignal::nextEdge() {
// Cue up the next pulse edge
  if ( pEdge->pNext ) {
    counter++;
    if ( counter > nEdges ) { return -4; }
    pEdge = pEdge->pNext;
    nextSignal ^= pEdge->signal;
    nextTime = pEdge->time;
    if ( nextTime == 0 ) { 
      nextTime = 0xFFFF; 
    }
    return 0;
  }
  return 0;
}


void PWMSignal::periodStart() {
  pEdge = pZero;
  nextSignal = pZero->signal;
  nextTime = 0xFFFF;
  counter = 0;
}


int8_t PWMSignal::printSignalList() {
  PulseEdge* pNode = pZero;
  uint8_t countout = 0;
  Serial.println();
  Serial.flush();
  Serial.print( "Signal: " );
  while ( pNode ) {
    Serial.print("0x"); Serial.print( pNode->signal, HEX ); Serial.print(":"); Serial.print( pNode->time, HEX ); Serial.print(" -> ");
    pNode = pNode->pNext;
    countout++;
    if ( countout > nEdges ) { 
      return -1; 
    }
  }
  return 0;
}


int8_t PWMSignal::insertEdge(uint16_t signal, uint16_t time) {
// Insert a pulse edge in the list in chronological order.
  if ( not time ) { 
    return 0;
  }
  PulseEdge* pNode = pZero;
  uint8_t timeout = 0;
  while ( pNode->pNext ) {
    if ( pNode->pNext->time == time ) {  
      pNode->pNext->signal |= signal;
      return 0;
    }
    if ( pNode->pNext->time > time ) { 
      PulseEdge* pNew = newEdge(signal, time);
      pNew->pNext = pNode->pNext;
      pNode->pNext = pNew;
      return 0;
    }
    pNode = pNode->pNext;

    timeout++;
    if ( timeout > 0x20 ) {
      return -1;
    }
  }
  pNode->pNext = newEdge(signal, time);
  return 0;
}


int8_t PWMSignal::removeEdge(uint16_t signal) {
// Remove a pulse edge signal from list.
  PulseEdge* x = pZero;
  PulseEdge* y = x->pNext;
  uint8_t timeout = 0;
  while ( y = x->pNext ) {
    if ( y->signal & signal ) { 
      y->signal &= ~signal;
      if ( not y->signal ) {
        deleteEdge(x, y);
      }
    }
    x = x->pNext;
    if ( not x ) { break; }
    timeout++;
    if ( timeout > 0x20 ) {
      return -1;
    }
  }
  return 0;
}


PulseEdge* PWMSignal::newEdge( uint16_t signal, uint16_t time ) { 
// Create a new pulse edge node.
  nEdges++;
  PulseEdge* pNode = new PulseEdge;
  pNode->signal = signal;
  pNode->time = time;
  pNode->pNext = nullptr;
  return pNode;
}


int8_t PWMSignal::deleteEdge( PulseEdge* edge, PulseEdge* parent ) {
// Delete node when there is no longer an edge signal on it.
  parent->pNext = edge->pNext;
  nEdges--;
  delete edge;
}


int8_t PWMSignal::cleanUp() {
  PulseEdge* pNode = pZero;
  uint8_t timeout = 0;
  while ( pNode->pNext ) {
    if ( not pNode->pNext->signal ) {
      PulseEdge* emptyNode = pNode->pNext;
      pNode->pNext = emptyNode->pNext;
      nEdges--;
      delete emptyNode;
      return 0;
    }
    pNode = pNode->pNext;
    timeout++;
    if ( timeout > 0x20 ) {
      return -1; 
    }
  }
  return 0;
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
  return 640UL + 4U * brightness + brightness / 2U * brightness / 4U;
}


uint16_t PWMSignal::pulseWidthCubic(uint16_t brightness) {
// Map brightness value to pulse width usisng cubic function f(x) = 4x + 2*(x/16)^3.
  return 640 + 4 * brightness + brightness / 2 * brightness / 256 * brightness / 8;
}


uint16_t PWMSignal::pulseWidthQuartic(uint32_t brightness) {
// Map brightness value to pulse width using quartic function f(x) = 4x + 2*(x/32)^4.
  return 640 + 4 * brightness + brightness / 2 * brightness / 256 * brightness / 256 * brightness / 8;
}


