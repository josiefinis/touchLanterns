/* 
======================================================================================================================================================
                    LANTERN COLLECTION
======================================================================================================================================================
*/ 

#include <cstdint>
#include "Graph.cpp"
#include "Lantern.cpp"
#include "PrintText.cpp"


LanternCollection::LanternCollection( const uint8_t numberOfLanterns, const uint16_t* adjacencyList ) 
  : Graph( numberOfLanterns, adjacencyList )
  , numberOfLanterns( numberOfLanterns )
{
  lanternArray[ numberOfLanterns ];
}


void LanternCollection::pushSensor( uint8_t idx, bool value ) {
  lanternArray[ idx ].pushSensor( value );
}


bool LanternCollection::update( uint8_t idx ) {
  bool retVal = 0;
  retVal |= lanternArray[ idx ].nextState();
  retVal |= lanternArray[ idx ].updateOutput();
  if ( lanternArray[ idx ].getOutput() == MAKE_TREE ) {
    makeTree( idx );
  }
  return retVal;
}


bool LanternCollection::changeBrightness( uint8_t idx ) {
  return lanternArray[ idx ].changeBrightness();
}


void LanternCollection::makeTree( uint8_t idx ) {
  uint8_t parentList[16];
  Tree tree = Tree( parentList, 16 );
  makeSpanningTree( &tree, idx);            
  for ( uint8_t i=0; i<16; i++ ) {
    Lantern* parent = &lanternArray[ tree.getParent( i ) ];
    Lantern* child = &lanternArray[ i ];
    child->setParent( parent );
    child->setState( WAIT );
    uint8_t delay = 3 + Random::pull( 2 );
    child->setOutput( SET_DELAY | delay );
    child->setDelay();
  }
}


void LanternCollection::print( void ) {
  for ( uint8_t i=0; i<16; i++; ) {
    printInput( lanternArray[ i ].getInput() );
    std::cout << " -> ";
    printState( lanternArray[ i ].getState() );
    std::cout << ":";
    printOutput( lanternArray[ i ].getOutput() );
    std::cout << "(" << lanternArray[ i ].getBrightness() << 
    std::cout << ")";
  }
