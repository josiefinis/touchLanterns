/* 
======================================================================================================================================================
                    LANTERN COLLECTION
======================================================================================================================================================
*/ 

#ifndef LANTERN_COLLECTION_CPP
#define LANTERN_COLLECTION_CPP

#include "Graph.cpp"
#include "Lantern.cpp"


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


void LanternCollection::burnDown( uint8_t idx ) {
  collection[ idx ].burnDown();
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
#endif
