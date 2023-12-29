/*
======================================================================================================================================================
                LIGHT
======================================================================================================================================================
*/

#ifndef LIGHT_H
#define LIGHT_H

#include "Global.h"
#include "Environment.h"


#define STABLE      0
#define TINY_STEP   1
#define SMALL_STEP  2
#define LARGE_STEP  3
#define HUGE_STEP   4
#define FLICKER     5
#define PULSE       6
#define RIPPLE      7

#define LIGHT_FULL  0xff

class Light {
    public:
        Light( uint8_t& reference );

        operator int() const; 
        
        uint8_t  getBrightness( void ) const;
        void     setBrightness( uint8_t value );
        uint16_t getBehaviour( void ) const;
        void     toggleSign( void );
        void     setSign( bool );
        bool     getSign( void ) const;
        void     setToDim( void );
        void     setToBrighten( void );
        void     setBehaviour( uint8_t value );
        bool     isBrightening( void ) const;
        bool     isDimming( void ) const;
        void changeBrightness( uint8_t floor=0, uint8_t ceil=0xff );
        void step( uint16_t stepsize, uint8_t floor=0, uint8_t ceil=0xff );
        void flicker( void );
        void pulse( void );
        void ripple( void );

    private:
        uint16_t reg;
        uint8_t& reference;
        friend class Bridge;
};
#endif
