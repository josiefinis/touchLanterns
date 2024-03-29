/*
======================================================================================================================================================
                                    PRIORITY QUEUE
======================================================================================================================================================
*/

#include "PriorityQueue.h"


PriorityQueue::PriorityQueue()
    : next( 0 )
    , size( 0 )
{ clear(); }


uint8_t PriorityQueue::dequeue( void ) 
{
    if ( isEmpty() ) 
    {
        return NONE;
    }
    return queueArray[ next++ ].value;
}


uint16_t PriorityQueue::peekPriority( void ) 
{
    if ( isEmpty() ) 
    {
        return 0xFFFF;
    }
    return queueArray[ next ].priority;
}


void PriorityQueue::insert( uint8_t value, uint16_t priority ) 
{
    changePriority( value, priority );
}


void PriorityQueue::remove( uint8_t value ) 
{
    uint8_t i = changePriority( value, 0xFFFF );
    queueArray[ i ].value = NONE;
    size--;
}


void PriorityQueue::clear( void ) 
{
    next = 0;
    size = 0;
    for ( uint8_t i=0; i<16; i++ ) 
    {
        queueArray[ i ].value = NONE;
        queueArray[ i ].priority = 0xFFFF;
    }
}


void PriorityQueue::refill( void ) 
{
    next = 0;
}


bool PriorityQueue::isEmpty( void ) 
{
    return next == size;
}


uint8_t PriorityQueue::changePriority( uint8_t value, uint16_t priority ) 
// Change the priority of signal edge and move it to new place in the queue.
{
    uint8_t i = 0;
    while ( value != queueArray[ i ].value )                 // Find 'value' in queue. If not found add a space at the end of the queue.
    {
        if ( ++i >= size ) 
        {
            i = size++;
            break;
        }
    }
    while ( i > 0 )                                                  // Move the space up 
    {
    if ( priority >= queueArray[ i - 1 ].priority ) { break; }
        queueArray[ i ] = queueArray[ i - 1 ]; i--;
    }
    while ( i < size - 1 ) 
    {
        if ( priority <= queueArray[ i + 1 ].priority ) { break; }      // or down, until it is in priority order.
        queueArray[ i ] = queueArray[ i + 1 ]; i++;
    }
    queueArray[ i ].value = value;
    queueArray[ i ].priority = priority;
    return i;
}


void PriorityQueue::print( void ) 
{
    if ( not size ) { return; }
    Serial.println();
    for ( uint8_t i=0; i<size; ++i ) 
    {
        Serial.print( queueArray[ i ].priority );
        Serial.print( "\t" );
        Serial.print( queueArray[ i ].value, HEX ); 
        Serial.println();
    }
}
