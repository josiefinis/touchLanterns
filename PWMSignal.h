/*
======================================================================================================================================================
                                PWM SIGNAL
======================================================================================================================================================
*/

#ifndef PWMSIGNAL_H
#define PWMSIGNAL_H

#define PWM_PERIOD      9856U // µs

#include "Global.h"
#include "Environment.h"

#include "PriorityQueue.h"


class PWMSignal 
{
    public:
        PWMSignal();
        void reset( void );

        void changeDuty( uint8_t index, uint8_t brightness );
        void  nextEdge( void );
        void startPeriod( void );
        uint16_t getSignal( void );
        uint16_t getTime( void );

    private:
        uint16_t signalAtTimeZero;
        uint16_t signal;
        uint16_t time;
        PriorityQueue edgeQueue;
        uint16_t pulseWidthLinear( uint8_t brightness );
        uint16_t pulseWidthQuadratic( uint16_t brightness );
        uint16_t pulseWidthCubic( uint16_t brightness );
        uint16_t pulseWidthQuartic( uint32_t brightness );
        friend class SerialMonitor;
};
#endif
