/*
======================================================================================================================================================
                STATE MACHINE
======================================================================================================================================================
*/

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "Arduino.h"
#include "TransitionMatrix.h"

#define DONT_CARE   0xFF

class StateMachine {
  public:
    StateMachine( const TransitionMatrix& matrix );
    bool next( uint8_t input );
    uint8_t getState( void );
    uint8_t getOutput( void );
    void setState( uint8_t value );
    void setOutput( uint8_t value );

  private:
    const TransitionMatrix& transitionMatrix;
    uint8_t state;
    uint8_t output;
};
#endif
