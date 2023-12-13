/*
======================================================================================================================================================
                STATE MACHINE
======================================================================================================================================================
*/

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <cstdint>
#include "TransitionMatrix.h"


class StateMachine {
  public:
    StateMachine( const TransitionMatrix& matrix );
    void pushInput( bool value );
    bool next( void );
    void setInput( uint8_t value );
    uint8_t getState( void );
    uint8_t getInput( void );
    uint8_t getOutput( void );

  private:
    const TransitionMatrix& transitionMatrix;
    uint8_t state;
    uint8_t input;
    uint8_t output;
    uint8_t classifyInput( uint8_t value );
};
#endif
