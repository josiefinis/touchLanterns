/*
======================================================================================================================================================
                                PWM SIGNAL
======================================================================================================================================================
*/

#include <cstdint>
#include "PWMSignal.h"
#include "PriorityQueue.cpp"


PWMSignal::PWMSignal()
  // Make zeroth node with signal = 0 and time = 0
  : signal( 0 )
  , time( 0 )
{
  edgeQueue.clear();
}


void PWMSignal::changeDuty(uint8_t index, uint8_t brightness) {
  // Change the PWM signal to reflect the new brightness for lantern with index 'index'.
  if ( brightness == 0xFF ) {
    signalAtTimeZero |= 1 << index;
    edgeQueue.remove( index );
    return;
  }
  if ( brightness == 0 ) {
    signalAtTimeZero &= ~( 1 << index );
    edgeQueue.remove( index );
    return;
  }

  uint16_t pulseWidth = pulseWidthQuadratic( brightness );
  if ( pulseWidth > PWM_PERIOD / 2 ) {
    signalAtTimeZero |= 1 << index;
    edgeQueue.insert( index, pulseWidth );
  }
  else {
    signalAtTimeZero &= ~( 1 << index );
    edgeQueue.insert( index, PWM_PERIOD - pulseWidth );
  }
  edgeQueue.print();
}


void PWMSignal::nextEdge( void ) {
  if ( edgeQueue.isEmpty() ) {
    time = 0xFFFF;
    signal = 0;
    return;
  }
  time = edgeQueue.peekPriority();
  while ( edgeQueue.peekPriority() == time ) {
    signal ^= 1 << edgeQueue.dequeue();
    if ( edgeQueue.isEmpty() ) {
      break;
    }
  }
}


void PWMSignal::startPeriod( void ) {
  time = 0;
  signal = signalAtTimeZero;
  edgeQueue.refill();
}


uint16_t PWMSignal::getSignal( void ) {
// Return the next signal to write to the register.
  return signal;
}


uint16_t PWMSignal::getTime( void ) {
// Return the time when the next signal is due (in microseconds after the start of the PWM period).
  return time;
}


// There is a choice of 4 functions that map brightness to pulse width such that
//                 f(1)  >  640 µs,         which gives lowest DC voltage that produces light from the LED candles,
//        f(x+1) - f(x) >=    4 µs,         so that f(x+1) and f(x) are distinct, 4 µs being the resolution of micros(),
//               f(256) == 9856 µs,         which is the PWM period.
// These are a linear, a quadratic, a cubic and a quartic function. 

uint16_t PWMSignal::pulseWidthLinear( uint8_t brightness ) {
// Map brightness value to pulse width using linear function. 
  return 640 + 36 * brightness;
}


uint16_t PWMSignal::pulseWidthQuadratic( uint16_t brightness ) {
// Map brightness value to pulse width usisng quadratic function f(x) = 4x + 2*(x/4)^2.
  return 640UL + 4U * brightness + brightness / 2U * brightness / 4U;
}


uint16_t PWMSignal::pulseWidthCubic( uint16_t brightness ) {
// Map brightness value to pulse width usisng cubic function f(x) = 4x + 2*(x/16)^3.
  return 640 + 4 * brightness + brightness / 2 * brightness / 256 * brightness / 8;
}


uint16_t PWMSignal::pulseWidthQuartic( uint32_t brightness ) {
// Map brightness value to pulse width using quartic function f(x) = 4x + 2*(x/32)^4.
  return 640 + 4 * brightness + brightness / 2 * brightness / 256 * brightness / 256 * brightness / 8;
}
