/* 
======================================================================================================================================================
                    LANTERN COLLECTION
======================================================================================================================================================
*/ 

#ifndef LANTERN_COLLECTION_CPP
#define LANTERN_COLLECTION_CPP

#include "LanternCollection.h"


LanternCollection::LanternCollection( const uint8_t size, const uint16_t* adjacencyList ) 
  : Graph( size, adjacencyList )
  , size( size )
{}


void LanternCollection::sense( uint8_t idx, bool value ) {
  collection[ idx ].sense( value );
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
  Tree tree = Tree( 16, parentList );
  makeSpanningTree( &tree, root );            
  printTree( tree );
  for ( uint8_t i=0; i<16; i++ ) {
    delay += 3 + Random::pull( 2 );                         // TODO delay should increase down the tree not with increasing index.
    if ( i == root ) { continue; }
    Lantern* parent = &collection[ tree.getParent( i ) ];
    Lantern* child = &collection[ i ];
    child->setParent( parent );
    child->setState( WAIT );
    child->setOutput( SET_DELAY | delay );                  // TODO SET_DELAY as implemented cannot set a delay longer than 0x3F. implement as log value? or shorter delay? 
    child->setDelay();                                      //      0x3F * 160 ms = 10 s which should be long enough.
  }
}


  void LanternCollection::printTree( const Tree& tree ) {
    Serial.println();
    for ( uint8_t i=0; i<16; i++ ) {
      Serial.print( tree.getParent( i ), HEX ); Serial.print( "\t" );
    } 
    Serial.println();
  }
  
  
  uint8_t LanternCollection::getInput( uint8_t idx ) {
    return collection[ idx ].prioritiseInput();
  }
  
  
  uint8_t LanternCollection::getState( uint8_t idx ) {
    return collection[ idx ].getState();
  }
  
  
  uint8_t LanternCollection::getOutput( uint8_t idx ) {
    return collection[ idx ].getOutput();
  }
#endif
