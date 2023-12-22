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
        uint16_t value;

        operator int() const; 
//        inline bool   operator == ( const Light&   other );
//        inline bool   operator != ( const Light&   other );
//        inline bool   operator <  ( const Light&   other );
//        inline bool   operator >  ( const Light&   other );
//        inline bool   operator <= ( const Light&   other );
//        inline bool   operator >= ( const Light&   other );
        
        
        inline uint8_t stepSize( void );
        void set( uint8_t value );
        void raise( void ); 
        void lower( void );
};
#endif
