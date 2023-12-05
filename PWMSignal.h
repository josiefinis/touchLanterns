#ifndef PWMSIGNAL_H
#define PWMSIGNAL_H

#include "Arduino.h"


class PriorityQueue {
  public:
    uint8_t dequeue();
    uint16_t peekTime();
    void insert( uint8_t index, uint16_t time );
    void remove( uint8_t index );
    void clear();
    void refill();
    bool isEmpty();
    void printQueue();

  private:
    struct QueueMember {
      uint8_t index;
      uint16_t time;
    };
    QueueMember queueArray[16];
    uint8_t next;
    uint8_t size;
    void changeTime( uint8_t index, uint16_t time );
};


class PWMSignal {
  public:
    PWMSignal();
    void reset();

    void changeDuty( uint8_t index, uint8_t brightness );
    void  nextEdge();
    void periodStart();
    uint16_t getSignal();
    uint16_t getTime();

  private:
    uint16_t signalAtTimeZero;
    uint16_t signal;
    uint16_t time;
    PriorityQueue edgeQueue;
    uint16_t pulseWidthLinear( uint8_t brightness );
    uint16_t pulseWidthQuadratic( uint16_t brightness );
    uint16_t pulseWidthCubic( uint16_t brightness );
    uint16_t pulseWidthQuartic( uint32_t brightness );
};
#endif
