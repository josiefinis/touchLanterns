// ============================================================================================================================================
//          QUEUE
// ============================================================================================================================================

// Queue implementation that tries to be frugal with memory and does not allocate memory on the heap.

#ifndef QUEUE_H
#define QUEUE_H

#include "Arduino.h"

class Queue {
  public:
    Queue( uint8_t* queueArray, uint8_t maxSize );
    bool isEmpty();
    bool enqueue( uint8_t value );
    uint8_t dequeue();
    void print();

  private:
    uint8_t* queue;
    uint8_t maxSize;
    uint8_t first;
    uint8_t size;
};
#endif
