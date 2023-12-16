/* 
======================================================================================================================================================
                    LANTERN COLLECTION
======================================================================================================================================================
*/ 

#ifndef LANTERN_COLLECTION_H
#define LANTERN_COLLECTION_H

#include <cstdint>
#include "Graph.h"
#include "Lantern.h"


class LanternCollection : public Graph {
  public:
    LanternCollection( const uint8_t numberOfLanterns, const uint16_t* adjacencyList );

    void pushSensor( uint8_t idx, bool value );
    bool update( uint8_t idx );
    bool changeBrightness( uint8_t idx );
    

  private:
    const uint8_t numberOfLanterns;
    Lantern lanternArray[];
    void makeTree( uint8_t index );
};
#endif
