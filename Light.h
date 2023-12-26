/*
======================================================================================================================================================
                LIGHT
======================================================================================================================================================
*/

#ifndef LIGHT_H
#define LIGHT_H

#include "Arduino.h"

#define LIGHT_FULL 0xFF

class Light {
    public:
        Light();

        operator int() const; 
        
        uint8_t  getBrightness( void ) const;
        void     setBrightness( uint8_t reg );
        uint16_t getRate( void ) const;
        void     toggleSign( void );
        void     setSign( bool );
        bool     getSign( void );
        void     setNegativeRate( void );
        void     setPositiveRate( void );
        void     setRate( uint8_t value );
        bool     isBrightening( void ) const;
        bool     isDimming( void ) const;
        void changeBrightness( uint8_t floor=0, uint8_t ceil=0xff );
        void raiseBrightness( uint8_t ceil=0xff ); 
        void lowerBrightness( uint8_t floor=0 );
        void flicker( void );
        void pulse( void );

    private:
        uint16_t reg;
        uint8_t reference;
};
#endif
