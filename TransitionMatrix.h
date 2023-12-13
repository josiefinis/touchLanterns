/*
======================================================================================================================================================
                TRANSITION MATRIX
======================================================================================================================================================
*/

#ifndef TRANSITION_MATRIX_H
#define TRANSITION_MATRIX_H

#include "Arduino.h"
#include "StateTransition.h"


class TransitionMatrix {
  public:
    uint8_t find( uint8_t state, uint8_t input );
    uint8_t getNextState( uint8_t index );
    uint8_t getNextOutput( uint8_t index );
  
  private:
    const StateTransition& matrix;
};
#endif
