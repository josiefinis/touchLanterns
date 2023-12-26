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
        
        inline uint8_t  getBrightness( void ) const;
        inline void     setBrightness( uint8_t reg );
        inline uint16_t getRate( void ) const;
        inline void     toggleSign( void );
        inline void     setSign( bool );
        inline bool     getSign( void );
        inline void     setNegativeRate( void );
        inline void     setPositiveRate( void );
        inline void     setRate( uint8_t value );
        inline bool     isBrightening( void ) const;
        inline bool     isDimming( void ) const;
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
