/*
======================================================================================================================================================
                                PRIORITY QUEUE
======================================================================================================================================================
*/

#include "PriorityQueue.h"
#include <iostream>


PriorityQueue::PriorityQueue()
  : next( 0 )
  , size( 0 )
{ clear(); }


uint8_t PriorityQueue::dequeue( void ) {
  if ( isEmpty() ) {
    return 16;
  }
  return queueArray[ next++ ].index;
}


uint16_t PriorityQueue::peekTime( void ) {
  if ( isEmpty() ) {
    return 0xFFFF;
  }
  return queueArray[ next ].time;
}


void PriorityQueue::insert( uint8_t index, uint16_t time ) {
  changeTime( index, time );
}


void PriorityQueue::print( void ) {
  for ( uint8_t i=0; i<size; i++ ) {
    std::cout << std::endl << ( int ) queueArray[i].index << "\t" << queueArray[i].time; 
  }
}


void PriorityQueue::remove( uint8_t index ) {
  changeTime( index, 0xFFFF );
  size--;
}


void PriorityQueue::clear( void ) {
  next = 0;
  size = 0;
  for ( uint8_t i=0; i<16; i++ ) {
    queueArray[ i ].index = 16;
    queueArray[ i ].time = 0xFFFF;
  }
}


void PriorityQueue::refill( void ) {
  next = 0;
}


bool PriorityQueue::isEmpty( void ) {
  return next == size;
}


void PriorityQueue::changeTime( uint8_t index, uint16_t time ) {
// Change the time of signal edge and move it to new place in the queue.
  uint8_t i = 0;
  while ( index != queueArray[ i ].index ) {
    if ( ++i >= size ) {
      i = size++;
      break;
    }
  }
  while ( i > 0 ) {
   if ( time >= queueArray[ i - 1 ].time ) { break; }
    queueArray[ i ] = queueArray[ i - 1 ]; i--;
  }
  while ( i < size - 1 ) {
    if ( time <= queueArray[ i + 1 ].time ) { break; }
    queueArray[ i ] = queueArray[ i + 1 ]; i++;
  }
  queueArray[ i ].index = index;
  queueArray[ i ].time = time;
}
