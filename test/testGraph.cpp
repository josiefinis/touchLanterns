#include "Graph.h"
#include "Graph.cpp"
#include <iostream>


int test_countAdjacent( void ) {
  uint16_t neighbourList[16] = 
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
  Graph graph = Graph( neighbourList, 16 );
  for ( uint8_t i=0; i<16; i++ ) {
    //std::cout << ( int ) graph.countAdjacent( i ) << std::endl;
    if ( graph.countAdjacent( i ) != numberAdjacent[ i ] ) {
      return -i;
    }
  }
  return 0;
}


void test_makeSpanningTree( void ) {
  uint16_t neighbourList[16] = 
  { 
    0x421 , 0x4320, 0x5310, 0x521 ,
    0x7610, 0x7632, 0xC854, 0xC854,
    0xA976, 0xDBA8, 0xB98 , 0xFA9 ,
    0xED76, 0xFEC9, 0xFDC , 0xEDB 
  };
  Graph graph = Graph( neighbourList, 16 );
  uint8_t parentList[16];
  Tree tree = Tree( parentList, 16 );
  graph.makeSpanningTree( &tree, 7 );
  for ( int i=0; i<16; i++ ) {
    std::cout << i << "\t" << ( int ) tree.getParent( i ) << std::endl;
  }
}


int main( void ) {
  std::cout << "test countAdjacent: " << test_countAdjacent() << std::endl;
  test_makeSpanningTree();

  return 0;
}
