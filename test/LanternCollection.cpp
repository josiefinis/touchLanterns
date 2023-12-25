/* 
======================================================================================================================================================
                    LANTERN COLLECTION
======================================================================================================================================================
*/ 

#include <cstdint>
#include "Graph.cpp"
#include "Lantern.cpp"

LanternCollection::LanternCollection( const uint8_t size, const uint16_t* adjacencyList ) 
  : Graph( size, adjacencyList )
  , size( size )
{}


      Lantern& LanternCollection::operator[](uint8_t idx)       { return collection[idx]; }
const Lantern& LanternCollection::operator[](uint8_t idx) const { return collection[idx]; }


bool LanternCollection::update( uint8_t idx, uint8_t sensorValue=NONE ) {
    switch ( collection[ idx ].update( sensorValue ) )
    {
        case NO_CHANGE:
            return 0;

        case MAKE_TREE:
            makeTree( idx );
            return 1;

        default:
            return 1;
    }
}


uint8_t LanternCollection::getBrightness( uint8_t idx ) 
{
    return collection[ idx ].getBrightness();
}


void LanternCollection::burnDown( uint8_t idx ) {
  collection[ idx ].burnDown();
}


void LanternCollection::makeTree( uint8_t root )
{
    uint8_t parentList[ 16 ];
    Tree tree = Tree( 16, parentList );
    makeSpanningTree( &tree, root );            
    //printTree( tree );
    for ( uint8_t i=0; i<16; i++ ) {
        if ( i == root ) { continue; }
        Lantern* parent = &collection[ tree.getParent( i ) ];
        Lantern* child = &collection[ i ];
        child->setParent( parent );
    }
}


// TestData LanternCollection::exportData( uint8_t idx ) {
//   TestData values;
//   values.input = collection[ idx ].prioritiseInput();
//   values.nextState = collection[ idx ].getState();
//   values.nextOutput = collection[ idx ].getOutput();
//   values.brightness = collection[ idx ].getBrightness();
//   return values;
// }
// 
// 
// void LanternCollection::print( uint8_t idx ) {
//   std::cout << std::hex << std::uppercase << ( int ) idx << ".";
//   printBrief( collection[ idx ].prioritiseInput(), collection[ idx ].getState(), collection[ idx ].getOutput() );
//   std::cout << std::dec;
// }
