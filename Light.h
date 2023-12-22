/*
======================================================================================================================================================
                LIGHT
======================================================================================================================================================
*/

#ifndef LIGHT_H
#define LIGHT_H

#include "Arduino.h"


class Light {
  public:
    const uint16_t value;

    inline Light& operator =  ( const Light& other );
    inline Light& operator ++ ( const Light& other );
    inline Light& operator -- ( const Light& other );
    inline bool operator == (const X& lhs, const X& rhs) { /* do actual comparison */ }
    inline bool operator != (const X& lhs, const X& rhs) { return !(lhs == rhs); }
    inline bool operator <  (const X& lhs, const X& rhs);
    inline bool operator >  (const X& lhs, const X& rhs);
    inline bool operator <= (const X& lhs, const X& rhs);
    inline bool operator >= (const X& lhs, const X& rhs);
};
#endif
