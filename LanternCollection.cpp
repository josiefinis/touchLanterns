/* 
======================================================================================================================================================
                    LANTERN COLLECTION
======================================================================================================================================================
*/ 
#include "LanternCollection.h"
#include "Global.h"


Idle    LanternCollection::IDLE;
Wake    LanternCollection::WAKE;
Full    LanternCollection::FULL;
Flicker LanternCollection::FLKR;
Auto    LanternCollection::AUTO;
Pause   LanternCollection::PAUS;
Pulse   LanternCollection::PULS;


LanternCollection::LanternCollection( const uint8_t size, const uint16_t* adjacencyList ) 
  : Graph( size, adjacencyList )
  , size( size )
{
    for ( int i=0; i<16; ++i )
    {
        collection[ i ].state = &IDLE;
    }
}


      Lantern& LanternCollection::operator[](uint8_t idx)       { return collection[idx]; }
const Lantern& LanternCollection::operator[](uint8_t idx) const { return collection[idx]; }


bool LanternCollection::updateCollection( uint8_t idx, uint8_t sensorValue=NONE ) {
// Update collection based on sensor input to the lantern at idx.

    Lantern& lantern = collection[ idx ];
    switch ( updateLantern( lantern, sensorValue ) )
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


uint8_t LanternCollection::updateLantern( Lantern& lantern, uint8_t sensorValue=NONE )
{
    if ( sensorValue == NONE )
    {
        return lantern.state->act( lantern );
    }
    if ( lantern.delay ) { --lantern.delay; } 
    lantern.input.push( sensorValue );
    uint8_t retValue = lantern.state->act( lantern );
    uint8_t next = lantern.state->getNext( lantern ); //TODO remove arg
    if ( next != *( lantern.state ) ) { 
        lantern.state->exit( lantern );
        changeState( lantern, next );
    }
    return retValue;
}


void LanternCollection::changeState( Lantern& lantern, uint8_t next )
{
    switch ( next )
    {
        case IDLE_ID:
            lantern.state = &IDLE;
            lantern.state->enter( lantern );
            break;

        case WAKE_ID:
            lantern.state = &WAKE;
            lantern.state->enter( lantern );
            break;

        case FULL_ID:
            lantern.state = &FULL;
            lantern.state->enter( lantern );
            break;

        case FLKR_ID:
            lantern.state = &FLKR;
            lantern.state->enter( lantern );
            break;

        case AUTO_ID:
            lantern.state = &AUTO;
            lantern.state->enter( lantern );
            break;

        case PAUS_ID:
            lantern.state = &PAUS;
            lantern.state->enter( lantern );
            break;

        case PULS_ID:
            lantern.state = &PULS;
            lantern.state->enter( lantern );
            break;
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
