/*
======================================================================================================================================================
                STATE MACHINE
======================================================================================================================================================
*/

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <cstdint>
#include "TransitionMatrix.h"

#define DONT_CARE   0xFF

class StateMachine {
  public:
    StateMachine( const TransitionMatrix& matrix );
    bool next( uint8_t input );
    uint8_t getState( void );
    uint8_t getOutput( void );

  private:
    const TransitionMatrix& transitionMatrix;
    uint8_t state;
    uint8_t output;
};
#endif
