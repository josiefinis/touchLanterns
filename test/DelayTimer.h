#ifndef DELAY_TIMER_H
#define DELAY_TIMER_H


#include <cstdint>
#include <iostream>
#include <chrono>
#include "Random.h"

#define SHORT_DELAY     24
#define LONG_DELAY      255


class DelayTimer
{
    public:
        DelayTimer();

        operator int() const;
        DelayTimer& operator++();
        DelayTimer operator++(int);
        DelayTimer& operator--();
        DelayTimer operator--(int);

        void set( uint8_t );
        void clear( void );
        bool isOver( void );
        void setRandom( uint8_t );

    private:
        uint8_t remaining; //  in units of 160 ms. 
};
#endif
