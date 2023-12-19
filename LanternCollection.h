/* 
======================================================================================================================================================
                    LANTERN COLLECTION
======================================================================================================================================================
*/ 

#ifndef LANTERN_COLLECTION_H
#define LANTERN_COLLECTION_H

#include "Arduino.h"
#include "Graph.h"
#include "Lantern.h"

#define COLLECTION_SIZE     16


struct TestData;

class LanternCollection : public Graph {
  public:
    LanternCollection( const uint8_t size, const uint16_t* adjacencyList );

    void sense( uint8_t idx, bool value );
    bool update( uint8_t idx );
    bool changeBrightness( uint8_t idx );
    uint8_t getBrightness( uint8_t idx );
    void burnDown( uint8_t idx);

    void printTree( const Tree& tree );
    uint8_t getInput( uint8_t idx );
    uint8_t getState( uint8_t idx );
    uint8_t getOutput( uint8_t idx );

  private:
    const uint8_t size;
    Lantern collection[ COLLECTION_SIZE ];
    void makeTree( uint8_t root );
};
#endif
