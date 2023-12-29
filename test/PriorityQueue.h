/*
======================================================================================================================================================
                                PRIORITY QUEUE 
======================================================================================================================================================
*/

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

//#include "Arduino.h"
#include <cstdint>

#define NONE 0xff
#define MAX_SIZE 0x10


class PriorityQueue {
  public:
    PriorityQueue();

    uint8_t dequeue( void );
    uint16_t peekPriority( void );
    void insert( uint8_t value, uint16_t priority );
    void remove( uint8_t value );
    void clear( void );
    void refill( void );
    bool isEmpty( void );
    void print( void );

  private:
    struct QueueMember {
      uint8_t value;
      uint16_t priority;
    };
    QueueMember queueArray[ MAX_SIZE ];
    uint8_t next;
    uint8_t size;
    uint8_t changePriority( uint8_t value, uint16_t priority );
};
#endif
