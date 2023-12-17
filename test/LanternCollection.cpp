/* 
======================================================================================================================================================
                    LANTERN COLLECTION
======================================================================================================================================================
*/ 

#include <cstdint>
#include "Graph.cpp"
#include "Lantern.cpp"
#include "PrintText.cpp"


LanternCollection::LanternCollection( const uint8_t size, const uint16_t* adjacencyList ) 
  : Graph( size, adjacencyList )
  , size( size )
{}


void LanternCollection::pushSensor( uint8_t idx, bool value ) {
  collection[ idx ].pushSensor( value );
}


bool LanternCollection::update( uint8_t idx ) {
  collection[ idx ].nextState();
  collection[ idx ].updateOutput();
  if ( collection[ idx ].getOutput() == MAKE_TREE ) {
    makeTree( idx );
  }
  return 1;
}


bool LanternCollection::changeBrightness( uint8_t idx ) {
  return collection[ idx ].changeBrightness();
}


uint8_t LanternCollection::getBrightness( uint8_t idx ) {
  return collection[ idx ].getBrightness();
}


void LanternCollection::makeTree( uint8_t root ) {
  uint8_t delay = 0;
  uint8_t parentList[ 16 ];
  Tree tree = Tree( parentList, 16 );
  makeSpanningTree( &tree, root );            
  for ( uint8_t i=0; i<16; i++ ) {
    delay += 3 + Random::pull( 2 );
    if ( i == root ) { continue; }
    Lantern* parent = &collection[ tree.getParent( i ) ];
    Lantern* child = &collection[ i ];
    child->setParent( parent );
    child->setState( WAIT );
    child->setOutput( SET_DELAY | delay );
    child->setDelay();
  }
}


TestData LanternCollection::exportData( uint8_t idx ) {
  TestData values;
  values.input = collection[ idx ].prioritiseInput();
  values.nextState = collection[ idx ].getState();
  values.nextOutput = collection[ idx ].getOutput();
  values.brightness = collection[ idx ].getBrightness();
  return values;
}


void LanternCollection::print( uint8_t idx ) {
  std::cout << std::hex << std::uppercase << ( int ) idx << ".";
  printBrief( collection[ idx ].prioritiseInput(), collection[ idx ].getState(), collection[ idx ].getOutput() );
  std::cout << std::dec;
}
