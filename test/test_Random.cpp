#include "Random.h"
#include "Random.cpp"
#include <iostream>


void test_pull( void ) {
  std::cout << "pull\t";
  for ( uint8_t i=0; i<16; i++ ) {
    std::cout << ( int ) Random::pull(4) << "\t";
  }
  std::cout << std::endl;
}


void test_urandom( void ) {
  std::cout << "urandom\t";
  for ( uint8_t i=0; i<16; i++ ) {
    std::cout << ( int ) Random::urandom(16) << "\t";
  }
  std::cout << std::endl;
}


void test_urandom_distribution( uint8_t randMax ) {
  uint32_t frequency[ randMax + 1 ];
  for ( uint8_t i=0; i<=randMax; i++ ) {
    frequency[ i ] = 0; 
  }
  for ( uint32_t i=0; i<0xFFFF; i++ ) {
    frequency[ Random::urandom( randMax ) ]++;
    frequency[ randMax ]++;
  }
  std::cout << "urandom" << std::endl;
  for ( uint32_t i=0; i<=randMax; i++ ) {
    std::cout << i << "\t" << frequency[ i ] << std::endl;
  }
}


int main( void ) {
  Random::seed( 0x14 );
  Random::seed( 0x24 );
  Random::seed( 0x05 );
  Random::seed( 0xA4 );

  test_pull();
  test_urandom();
  for ( uint8_t i=2; i<10; i++ ) {
    test_urandom_distribution( i );
  }


  return 0;
}
