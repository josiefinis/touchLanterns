
/* 
======================================================================================================================================================
                    MONITOR
======================================================================================================================================================
*/ 
#include "Arduino.h"
#include "LanternCollection.h"
#include "Lantern.h"
#include "PWMSignal.h"

#define IDLE_ID     0
#define WAKE_ID     1
#define FULL_ID     2
#define FLKR_ID     3
#define AUTO_ID     4
#define PAUS_ID     5

#define STABLE      0
#define TINY_STEP   1
#define SMALL_STEP  2
#define LARGE_STEP  3
#define HUGE_STEP   4
#define FLICKER     5
#define PULSE       6

class SerialMonitor
{
    public:
        void printIndices( void );
        void printSpace( void );
        void printLine( LanternCollection& collection );
        void quickPrintLine( LanternCollection& collection );
        void printLight( LanternCollection& collection );
        void printRawInput( SensorInput& input );
        void printDelay( uint8_t value ); 
        void printInput( const SensorInput& input ); 
        void printState( uint8_t stateID );
        void printParent( uint8_t idx );
        void printBrightness( Lantern& lantern );
        void printBehaviour( Lantern& lantern );
        void printSign( Lantern& lantern );
        void printPWM( PWMSignal& pwm );
};
