// ============================================================================================================================================
//          QUEUE
// ============================================================================================================================================

#include "Queue.h"

Queue::Queue( uint8_t* queue, uint8_t maxSize ) 
  : queue( queue ) 
  , maxSize( maxSize ) 
  , first( 0 ) 
  , size( 0 ) 
{}


bool Queue::enqueue( uint8_t value ) {
// Put value at back of queue.
  if ( size >= maxSize ) {
    return 0;
  }
  queue[ ( first + size ) % maxSize ] = value;
  size++;
  return 1;
}


uint8_t Queue::dequeue() {
// Take value from front of queue.
  if ( isEmpty() ) {
    return NONE;
  }
  size--;
  first %= maxSize;
  return queue[ first++ ];
}


bool Queue::isEmpty() {
// True if queue has no nodes.
  return size == 0;
}


#if MONITOR_MAKE_TREE
void Queue::print() {
// Print the queue.
  for ( uint8_t i=0; i < maxSize; i++ ) {
    Serial.print( queue[ ( first + i ) % maxSize ] ); 
    Serial.print( "->" );
  }
  Serial.println();
}
#endif
