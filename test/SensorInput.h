/*
======================================================================================================================================================
                    SENSOR_INPUT
======================================================================================================================================================
*/

#ifndef SENSOR_INPUT_H
#define SENSOR_INPUT_H

#include <cstdint>
#include <iostream>



#define MEDIUM_TOUCH    0xff0f
#define LONG_TOUCH      0xffff
#define RISING_EDGE     0x0301
#define FALLING_EDGE    0x0302


class SensorInput 
{
    public:
        SensorInput();
        bool operator == ( const uint16_t& other ) const;
        bool operator != ( const uint16_t& other ) const;
        void push( bool value );

    private:
        uint8_t reg;
};
#endif
