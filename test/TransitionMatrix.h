/*
======================================================================================================================================================
                TRANSITION MATRIX
======================================================================================================================================================
*/

#ifndef TRANSITION_MATRIX_H
#define TRANSITION_MATRIX_H

#include <cstdint>
#include "StateTransition.h"

#define NOT_FOUND 0xFF;

class TransitionMatrix {
  public:
    TransitionMatrix( const uint8_t rows, const StateTransition* matrix );
    const uint8_t rows;
    const StateTransition* matrix;

    uint8_t find( uint8_t const state, const uint8_t input ) const;
    uint8_t getNextState( uint8_t index ) const;
    uint8_t getNextOutput( uint8_t index ) const;
  
};
#endif
