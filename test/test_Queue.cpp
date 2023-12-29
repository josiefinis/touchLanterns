#include "Queue.h"
#include "Queue.cpp"
#include <iostream>


int test_enqueue( void ) {
  uint8_t qArray[10];
  Queue q = Queue( qArray, 10 );
  if ( not q.enqueue( 1 ) ) {
    return -1;
  }
  for ( uint8_t i=0; i<9; i++ ) {
    if ( not q.enqueue( i ) ) {
      return -2;
    }
  }
  for ( uint8_t i=10; i<20; i++ ) {
    if ( q.enqueue( i ) ) {
      return -3;
    }
  }
  for ( uint8_t i=0; i<5; i++ ) {
    q.dequeue();
  }
  for ( uint8_t i=0; i<5; i++ ) {
    if ( not q.enqueue( i ) ) {
      return -4;
    }
  }
  for ( uint8_t i=10; i<20; i++ ) {
    if ( q.enqueue( i ) ) {
      return -5;
    }
  }
  return 0;
}


int test_dequeue( void ) {
  uint8_t qArray[10];
  Queue q = Queue( qArray, 10 );
  for ( uint8_t i=0; i<10; i++ ) {
    q.enqueue( i );
  }
  for ( uint8_t i=0; i<10; i++ ) {
    if ( q.dequeue() != i ) {
      return -1;
    }
  }
  for ( uint8_t i=0; i<10; i++ ) {
    if ( q.dequeue() != NONE ) {
      return -2;
    }
  }
  return 0;
}


int test_isEmpty( void ) {
  uint8_t qArray[10];
  Queue q = Queue( qArray, 10 );
  if ( not q.isEmpty() ) {
    return -1;
  }
  for ( uint8_t i=0; i<20; i++ ) {
    q.enqueue( i );
    if ( q.isEmpty() ) {
      return -2;
    }
  }
  for ( uint8_t i=0; i<9; i++ ) {
    q.dequeue();
    if ( q.isEmpty() ) {
      return -3;
    }
  }
  q.dequeue();
  if ( not q.isEmpty() ) {
    return -4;
  }
  return 0;
}


int main( void ) {
  std::cout << "\nTest enqueue: " << test_enqueue();
  std::cout << "\nTest dequeue: " << test_dequeue();
  std::cout << "\nTest isEmpty: " << test_isEmpty();
  std::cout << "\n";
  uint8_t qArray[10];
  Queue q = Queue( qArray, 8 );
  for ( uint8_t i=0; i<12; i++ ) {
    std::cout << ( int ) q.enqueue( i ) << ", ";
  }
  std::cout << "\n";
  for ( uint8_t i=0; i<12; i++ ) {
    std::cout << ( int ) q.dequeue() << ", ";
  }

  return 0;
}
