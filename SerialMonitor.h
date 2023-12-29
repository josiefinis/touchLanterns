
/* 
======================================================================================================================================================
                    MONITOR
======================================================================================================================================================
*/ 
#include "Global.h"
#include "Environment.h"

#include "LanternCollection.h"
#include "Lantern.h"
#include "PWMSignal.h"

#define MONITOR_INTERVAL                    160 // ms
#define MONITOR_QUICK_PRINT                 0
#define MONITOR_TIMINGS                     0
#define MONITOR_RAW_INPUT                   0
#define MONITOR_NORMALISED_INPUT            0
#define MONITOR_INPUT                       0
#define MONITOR_STATE                       0
#define MONITOR_BRIGHTNESS                  0
#define MONITOR_LIGHT_BEHAVIOUR             0
#define MONITOR_PARENT                      0
#define MONITOR_MAKE_TREE                   0
#define MONITOR_PWM_LIST                    0
#define MONITOR_PWM                         0
#define MONITOR_DELAY                       0
#define SERIAL_ON MONITOR_INPUT | MONITOR_PWM | MONITOR_TIMINGS | MONITOR_RAW_INPUT | MONITOR_NORMALISED_INPUT | MONITOR_STATE | MONITOR_PWM_LIST | MONITOR_BRIGHTNESS | MONITOR_LIGHT_BEHAVIOUR


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
