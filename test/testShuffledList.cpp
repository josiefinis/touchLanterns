#include "ShuffledList.h"
#include "ShuffledList.cpp"
#include <iostream>


void test_next( void ) {
  uint16_t testDataList[] = { 0x1234, 0x0234, 0x1034, 0x1230, 0x1230, 0x230, 0x230, 0x230 };
  uint8_t testDataSize[] = { 4, 4, 4, 4, 3, 3, 2, 1 };
  for ( uint8_t i=0; i<8; i++ ) {
    ShuffledList list = ShuffledList( testDataList[i], testDataSize[i] );
    std::cout << "( " << std::hex << testDataList[i] << ", " << ( int ) testDataSize[i] << " )";
    for ( uint8_t i=0; i<5; i++ ) {
      std::cout << std::hex << ( int ) list.next() << ", ";
    }
    std::cout << std::endl;
  }
}


void test_shuffle( uint8_t size ) {
  uint32_t frequency[ size ][ size + 1 ];
  for ( uint8_t i=0; i<size; i++ ) {
    for ( uint8_t j=0; j<=size; j++ ) {
      frequency[ i ][ j ] = 0;
    }
  }
  for ( uint32_t i=0; i<0xFFFF; i++ ) {
    ShuffledList list = ShuffledList( 0x3210, size );
    for ( int j=0; j<size; j++ ) {
      uint8_t x = list.next();
      frequency[ x ][ j ]++;
      frequency[ x ][ size ]++;
    }
  }
  for ( int i=0; i<size; i++ ) {
    std::cout << std::dec << i << ": ";
    for ( int j=0; j<=size; j++ ) {
      std::cout << frequency[ i ][ j ] << "\t";
    }
    std::cout << std::endl;
  }
}


int main( void ) {
  Random::seed( 0x14 );
  Random::seed( 0x24 );
  Random::seed( 0x05 );
  Random::seed( 0xA4 );
  test_next();
  test_shuffle(4);
  test_shuffle(3);
  test_shuffle(2);
  test_shuffle(1);

  return 0;
}
