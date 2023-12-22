/*
======================================================================================================================================================
                LIGHT
======================================================================================================================================================
*/

#ifndef LIGHT_H
#define LIGHT_H

#include <cstdint>


class Light {
    public:
        Light();

        operator int() const; 
        
        inline uint8_t  getBrightness( void ) const;
        inline void     setBrightness( uint8_t reg );
        inline uint16_t getRateOfChange( void ) const;
        inline void     toggleUpDown( void );
        inline void     setRateOfChange( uint8_t value );
        inline bool     isBrightening( void ) const;
        inline bool     isDimming( void ) const;
        void changeBrightness( uint8_t floor, uint8_t ceil );
        void raiseBrightness( uint8_t ceil ); 
        void lowerBrightness( uint8_t floor );

    private:
        uint16_t reg;
};
#endif
