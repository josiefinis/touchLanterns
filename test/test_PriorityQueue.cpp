#include "PriorityQueue.h"
#include "PriorityQueue.cpp"
#include <iostream>

const uint16_t testTime[ 16 ] = { 13000, 7000, 6000, 8000, 14000, 3000, 2000, 4000, 12000, 11000, 0, 1000, 5000, 9000, 10000, 15000 };
const uint8_t testIdx[ 16 ] = { 13, 7, 6, 8, 14, 3, 2, 4, 12, 11, 0, 1, 5, 9, 10, 15 };
const uint8_t ansIdx[ 16 ] = { 3, 14, 11, 0, 12, 8, 9, 10, 5, 6, 7, 2, 4, 15, 1, 13 };


void test_dequeue( void ) {
  std::cout << "Test dequeue, ";
  int errorCount = 0;
  PriorityQueue p;
  for ( int i=0; i<256; i++ ) {
    uint8_t value = p.dequeue();
    if ( value != NONE ) {
      errorCount++;
      std::cout << std::endl << ( int ) value << " != " << NONE;
    }
  }
  std::cout <<  errorCount << " errors." << std::endl;
}


void test_peekTime( void ) {
  std::cout << "Test peekTime, ";
  int errorCount = 0;
  PriorityQueue p;
  if ( p.peekTime() != 0xFFFF ) { errorCount++; }
  for ( uint8_t i=0; i<16; i++ ) {
    p.insert( testIdx[ i ], testTime[ i ] );
  }
  for ( int i=0; i<16; i++ ) {
    if ( p.peekTime() != 1000 * i ) {
      errorCount++;
      std::cout << std::endl << p.peekTime() << " != " << 1000 * i;
    }
    p.dequeue();
  }
  std::cout <<  errorCount << " errors." << std::endl;
}


void test_insert( void ) {
  std::cout << "Test insert, ";
  int errorCount = 0;
  PriorityQueue p;
  for ( uint8_t i=0; i<16; i++ ) {
    p.insert( testIdx[ i ], testTime[ i ] );
  }
  for ( int i=0; i<16; i++ ) {
    uint8_t value = p.dequeue();
    if ( value != i ) {
      errorCount++;
      std::cout << std::endl << ( int ) value << " != " << i;
    }
  }
  for ( uint8_t i=0; i<16; i++ ) {
    p.insert( testIdx[ i ], testTime[ 15 - i ] );
  }
  p.refill();
  for ( int i=0; i<16; i++ ) {
    uint8_t value = p.dequeue();
    if ( value != ansIdx[ i ] ) {
      errorCount++;
      std::cout << std::endl << ( int ) value << " != " << ansIdx[ i ];
    }
  }
  for ( uint8_t i=0; i<16; i++ ) {
    p.insert( testIdx[ i ], testTime[ i ] );
  }
  p.refill();
  for ( int i=0; i<16; i++ ) {
    uint8_t value = p.dequeue();
    if ( value != i ) {
      errorCount++;
      std::cout << std::endl << ( int ) value << " != " << i;
    }
  }
  if ( errorCount ) { std::cout << std::endl; }
  std::cout << errorCount << " errors." << std::endl;
}


void test_remove() {
  std::cout << "Test remove, ";
  int errorCount = 0;
  PriorityQueue p;
  for ( uint8_t i=0; i<16; i++ ) {
    p.insert( testIdx[ i ], testTime[ i ] );
  }
  p.remove( 4 );
  p.remove( 15 );
  p.remove( 0 );
  p.remove( 4 );
  p.remove( 0 );

  for ( int i=0; i<16; i++ ) {
    p.print();
    if ( i == 0 or i == 4 or i == 15 ) { continue; }
    uint8_t value = p.dequeue();
    if ( value == 0 or value == 4 or value == 15 ) { errorCount++; continue; }
    if ( value != i ) {
      errorCount++;
      std::cout << std::endl << ( int ) value << " != " << i;
    }
  }
  if ( errorCount ) { std::cout << std::endl; }
  std::cout << errorCount << " errors." << std::endl;
}


void test_clear( void ) {
  std::cout << "Test clear, ";
}


void test_refill( void ) {
  std::cout << "Test refill, ";
  PriorityQueue p;
  if ( not p.isEmpty() ) { std::cout << "Failed on newly initialised queue."; }
  p.insert( 0, 1000 );
  if ( p.isEmpty() ) { std::cout << "Failed on queue with 1 element."; }
  
}


void test_isEmpty( void ) {
  std::cout << "Test isEmpty, ";
  PriorityQueue p;
  if ( not p.isEmpty() ) { std::cout << "Failed on newly initialised queue."; }
  p.insert( 0, 1000 );
  if ( p.isEmpty() ) { std::cout << "Failed on queue with 1 element."; }
  std::cout << p.dequeue();
  if ( not p.isEmpty() ) { std::cout << "Failed after dequeue."; }
  p.refill();
  if ( p.isEmpty() ) { std::cout << "Failed after refill."; }
}






int main( void ) {
  test_dequeue();
  test_insert();
  test_peekTime();
//  test_remove();
  test_isEmpty();

  return 0;
}
