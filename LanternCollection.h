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
#define NONE                0xff


struct TestData;

class LanternCollection : public Graph {
  public:
    LanternCollection( const uint8_t size, const uint16_t* adjacencyList );

        Lantern& operator[](uint8_t idx);
  const Lantern& operator[](uint8_t idx) const;

        bool update( uint8_t idx, uint8_t sensorValue=NONE );
        uint8_t getBrightness( uint8_t idx );
        void burnDown( uint8_t idx);

        void print( uint8_t idx );
        TestData exportData( uint8_t idx );

  private:
    const uint8_t size;
    Lantern collection[ COLLECTION_SIZE ];
    void makeTree( uint8_t root );
    friend class Bridge;
};
#endif
