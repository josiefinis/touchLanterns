/*
======================================================================================================================================================
                STATE TRANSITION
======================================================================================================================================================
*/

#ifndef STATE_TRANSITION_H
#define STATE_TRANSITION_H

#include "Arduino.h"


class StateTransition {
  public:
    StateTransition( const uint8_t state, const uint8_t input, const uint8_t nextState, const uint8_t nextOutput );
    bool operator < ( const StateTransition& object );
    bool operator > ( const StateTransition& object );

  private:
    const uint8_t state;
    const uint8_t input;
    const uint8_t nextState;
    const uint8_t nextOutput;            
};
