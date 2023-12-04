#ifndef PWMSIGNAL_H
#define PWMSIGNAL_H

#include "Arduino.h"

class PulseEdge {
  public:
    uint16_t signal;
    uint16_t time;
    PulseEdge* pNext;
};

class PWMSignal {
  public:
    PWMSignal();
    void reset();

    int8_t changeDuty( uint8_t index, uint8_t brightness );
    uint16_t getSignal();
    uint16_t getTime();
    int8_t nextEdge();
    void periodStart();
    int8_t printSignalList();

  private:
    uint8_t counter;
    uint8_t nEdges;
    PulseEdge* pZero;
    PulseEdge* pEdge;
    uint16_t nextSignal;
    uint16_t nextTime;

    uint16_t pulseWidthLinear( uint8_t brightness );
    uint16_t pulseWidthQuadratic( uint16_t brightness );
    uint16_t pulseWidthCubic( uint16_t brightness );
    uint16_t pulseWidthQuartic( uint32_t brightness );

    int8_t insertEdge( uint16_t signal, uint16_t time );
    int8_t removeEdge( uint16_t signal );
    PulseEdge* newEdge( uint16_t signal, uint16_t time );
    int8_t deleteEdge( PulseEdge* edge, PulseEdge* parent );
    int8_t cleanUp();
};
#endif
