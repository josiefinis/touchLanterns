#ifndef STOPWATCH_H
#define STOPWATCH_H

#include "Global.h"
#include "Environment.h"


class Stopwatch {

  public:
    void tick();
    void tock();
    uint32_t getMean();
    uint32_t getMax();

  private:
    uint32_t tickTime;
    uint8_t nSamples;
    uint32_t sum;
    uint32_t mean;
    uint32_t maximum;
};
#endif
