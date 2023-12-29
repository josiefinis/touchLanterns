#include "Graph.h"
#include "Graph.cpp"
#include <iostream>


int test_countAdjacent( void ) {
  const uint16_t neighbourList[16] = 
  { 
    0x421 , 0x4320, 0x5310, 0x521 ,
    0x7610, 0x7632, 0xC854, 0xC854,
    0xA976, 0xDBA8, 0xB98 , 0xFA9 ,
    0xED76, 0xFEC9, 0xFDC , 0xEDB 
  };
  uint8_t numberAdjacent[16] = 
  { 
    3, 4, 4, 3, 
    4, 4, 4, 4,
    4, 4, 3, 3,
    4, 4, 3, 3
  };
  Graph graph = Graph( 16, neighbourList );
  for ( uint8_t i=0; i<16; i++ ) {
    if ( graph.countAdjacent( i ) != numberAdjacent[ i ] ) {
      return -i;
    }
  }
  return 0;
}


void test_makeSpanningTree( uint8_t correctTree[16] ) {
  std::cout << "Test makeSpanningTree. ";
  int errorCount = 0;
  uint16_t neighbourList[16] = 
  { 
    0x421 , 0x4320, 0x5310, 0x521 ,
    0x7610, 0x7632, 0xC854, 0xC854,
    0xA976, 0xDBA8, 0xB98 , 0xFA9 ,
    0xED76, 0xFEC9, 0xFDC , 0xEDB 
  };
  Graph graph = Graph( 16, neighbourList );
  uint8_t parentList[16];
  Tree tree = Tree( parentList, 16 );
  graph.makeSpanningTree( &tree, 7 );
  for ( int i=0; i<16; i++ ) {
    if ( tree.getParent( i ) != correctTree[ i ] ) {
      std::cout << std::endl << "failure: lantern " << i << " has parent " << ( int ) tree.getParent( i ) << " (expected " << ( uint8_t ) correctTree[ i ] << std::endl;
      errorCount++;
    }
  }
  if ( errorCount ) {
    std::cout << "Failed. " << errorCount << " errors." << std::endl;
    return;
  }
  std::cout << " OK." << std::endl;
  return;
}


int main( void ) {
  std::cout << "test countAdjacent: ";
  if ( not test_countAdjacent() ) { 
    std::cout << "OK."; 
  } 
  else { 
    std::cout << "Failed."; 
  } 
  std::cout << std::endl;
  uint8_t correctTree[16] = { 4 , 4 , 5 , 5 , 7 , 7 , 4 , 255 , 7 , 8 , 8 , 10 , 7 , 12 , 12 , 14 };
  test_makeSpanningTree( correctTree );

  return 0;
}
