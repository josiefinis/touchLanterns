
/* 
======================================================================================================================================================
                    MONITOR
======================================================================================================================================================
*/ 
#include "Arduino.h"
#include "LanternCollection.h"
#include "Lantern.h"

#define IDLE_ID     0
#define WAKE_ID     1
#define FULL_ID     2
#define FLKR_ID     3
#define AUTO_ID     4
#define PAUS_ID     5

class SerialMonitor
{
    public:
        void printIndices();
        void printLine( LanternCollection& collection );
        void printRawInput( SensorInput& input );
        void printDelay( uint8_t value ); 
        void printInput( const SensorInput& input ); 
        void printState( uint8_t stateID );
        void printParent( uint8_t idx );
        void printBrightness( Lantern& lantern );
};
