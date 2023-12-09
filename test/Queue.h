// ============================================================================================================================================
//          QUEUE
// ============================================================================================================================================

// Queue implementation that tries to be frugal with memory and does not allocate memory on the heap.

#ifndef QUEUE_H
#define QUEUE_H

#include <cstdint>

#define NONE    0xFF

class Queue {
  public:
    Queue( uint8_t* queue, uint8_t maxSize );
    bool enqueue( uint8_t value );
    uint8_t dequeue();
    bool isEmpty();
    void print();

  private:
    uint8_t* queue;
    uint8_t maxSize;
    uint8_t first;
    uint8_t size;
};
#endif
