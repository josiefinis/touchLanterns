#ifndef PWMSIGNAL_H
#define PWMSIGNAL_H

#include "Arduino.h"

class pulseEdge {
  public:
    uint16_t signal;
    uint16_t time;
    pulseEdge* pNext;
};

class PWMSignal {
  public:
    PWMSignal();

    void changeDuty(uint8_t index, uint8_t brightness);
    uint16_t getSignal();
    uint16_t getTime();
    void nextEdge();
    void periodStart();
    void printSignalList();

  private:
    pulseEdge* pZero;
    pulseEdge* pEdge;
    uint16_t nextSignal;
    uint16_t nextTime;

    uint16_t pulseWidthLinear(uint8_t brightness);
    uint16_t pulseWidthQuadratic(uint16_t brightness);
    uint16_t pulseWidthCubic(uint16_t brightness);
    uint16_t pulseWidthQuartic(uint32_t brightness);

    void insertEdge(uint16_t signal, uint16_t time);
    void removeEdge(uint16_t signal);
    pulseEdge* newEdge(uint16_t signal, uint16_t time);
    void deleteEdge(pulseEdge* pDeadEdge);
};
#endif
