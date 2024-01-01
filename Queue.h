/*
============================================================================================================================================
                         QUEUE
============================================================================================================================================

Queue implementation that tries to be frugal with memory and does not allocate memory on the heap.
*/

#ifndef QUEUE_H
#define QUEUE_H

#include "Global.h"
#include "Environment.h"


#define NONE    0xff

class Queue 
{
    public:
        Queue( uint8_t* queue, uint8_t maxSize );
        bool enqueue( uint8_t value );
        uint8_t dequeue();
        bool isEmpty();

    private:
        uint8_t* queue;
        uint8_t maxSize;
        uint8_t first;
        uint8_t size;
};
#endif
