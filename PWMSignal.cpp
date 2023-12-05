#include "Global.h"
#include "PWMSignal.h"
#include "Arduino.h"


PWMSignal::PWMSignal() {
  // Make zeroth node with signal = 0 and time = 0
  signal = 0;
  time = 0;
  edgeQueue.clear();
}


void PWMSignal::changeDuty(uint8_t index, uint8_t brightness) {
  // Change the PWM signal to reflect the new brightness for lantern with index 'index'.
  if ( brightness == 0xFF ) {
    signalAtTimeZero |= 1 << index;
    edgeQueue.remove( index );
    return 0;
  }
  if ( brightness == 0 ) {
    signalAtTimeZero &= ~( 1 << index );
    edgeQueue.remove( index );
    return 0;
  }

  uint16_t pulseWidth = pulseWidthQuadratic(brightness);
  if ( pulseWidth > PWM_PERIOD / 2 ) {
    signalAtTimeZero |= 1 << index;
    edgeQueue.insert( index, pulseWidth );
  }
  else {
    signalAtTimeZero &= ~( 1 << index );
    edgeQueue.insert( index, PWM_PERIOD - pulseWidth );
  }
}


void PWMSignal::nextEdge() {
  if ( edgeQueue.isEmpty() ) {
    time = 0xFFFF;
    signal = 0;
    return 0;
  }
  time = edgeQueue.peekTime();
  while ( edgeQueue.peekTime() == time ) {
    signal |= 1 << edgeQueue.dequeue();
    if ( edgeQueue.isEmpty() ) {
      break;
    }
  }
}


void PWMSignal::periodStart() {
  time = 0;
  signal = signalAtTimeZero;
  edgeQueue.refill();
}


uint16_t PWMSignal::getSignal() {
// Return the next signal to write to the register.
  return signal;
}


uint16_t PWMSignal::getTime() {
// Return the time when the next signal is due (in microseconds after the start of the PWM period).
  return time;
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


// ==============================================================================================================
// PRIORITY QUEUE
// ==============================================================================================================


uint8_t PriorityQueue::dequeue() {
  if ( next == size ) {
  }
  return queueArray[next++].index;
}


uint16_t PriorityQueue::peekTime() {
  return queueArray[next].time;
}


void PriorityQueue::insert( uint8_t index, uint16_t time ) {
  changeTime( index, time );
}


void PriorityQueue::remove( uint8_t index ) {
  changeTime( index, 0xFFFF );
  size--;
}


void PriorityQueue::clear() {
  next = 0;
  size = 0;
  for ( uint8_t i=0; i<16; i++ ) {
    queueArray[i].index = 0;
    queueArray[i].time = 0xFFFF;
  }
}


void PriorityQueue::refill() {
  next = 0;
}


bool PriorityQueue::isEmpty() {
  return next == size;
}


#if MONITOR_PWM_LIST
void PriorityQueue::printQueue() {
  Serial.println();
  uint8_t i;
  while ( i++ < size ) {
    Serial.print( queueArray[i].index, HEX ); Serial.print(":"); Serial.print( queueArray[i].time, DEC ); Serial.print(" -> ");
  }
}
#endif 


void PriorityQueue::changeTime( uint8_t index, uint16_t time ) {
// Change the time of signal edge and move it to new place in the queue.
  uint8_t i = 0;
  while ( i < size ) {      // Find the index in the queue.
    if ( queueArray[i].index == index ) {
      break;
    }
    i++;
  }
  if ( i == size - 1 ) {    // Index not found, append to queue.
    i = size++;
    queueArray[i].index = index;
    queueArray[i].time = time;
  }
  while ( i > 0 ) {         // Move up queue, shifting each signal down one place, until an earlier signal than 'time' is reached.
    i--;
    if ( queueArray[i].time < time ) {
      break;
    }
    queueArray[i+1] = queueArray[i];
  }
  while ( i < size ) {      // Move down queue, shifting each signal up one place, until a later signal than 'time' is reached.
    i++;
    if ( queueArray[i].time > time ) {
      break;
    }
    queueArray[i-1] = queueArray[i];
  }
  queueArray[i].index = index;
  queueArray[i].time = time;
}
