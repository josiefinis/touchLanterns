#include "Stopwatch.h"

void Stopwatch::tick() {
  tickTime = micros();
}


void Stopwatch::tock() {
  uint32_t time = micros() - tickTime;
  sum += time;
  nSamples++;
  if ( nSamples == 0 ) {
    mean = sum / 256;
    sum = 0;
  }
  if ( time > maximum ) {
    maximum = time;
  }
}


uint32_t Stopwatch::getMean() { 
  return mean; 
}


uint32_t Stopwatch::getMax() { 
  uint32_t buffer = maximum;
  maximum = 0;
  return buffer;  
}
