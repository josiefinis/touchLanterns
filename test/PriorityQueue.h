/*
======================================================================================================================================================
                                PRIORITY QUEUE 
======================================================================================================================================================
*/

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

//#include "Arduino.h"
#include <cstdint>


class PriorityQueue {
  public:
    PriorityQueue();

    uint8_t dequeue( void );
    uint16_t peekTime( void );
    void insert( uint8_t index, uint16_t time );
    void remove( uint8_t index );
    void clear( void );
    void refill( void );
    bool isEmpty( void );
    void print( void );

  private:
    struct QueueMember {
      uint8_t index;
      uint16_t time;
    };
    QueueMember queueArray[ 16 ];
    uint8_t next;
    uint8_t size;
    void changeTime( uint8_t index, uint16_t time );
};
#endif
