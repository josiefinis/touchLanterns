/*
======================================================================================================================================================
                STATE TRANSITION
======================================================================================================================================================

A single state transition 
    T: state, input --> nextState, nextOutput.

Instances ordered for comparison by state then input.
*/

#ifndef STATE_TRANSITION_H
#define STATE_TRANSITION_H

#include "Arduino.h"


class StateTransition {
  public:
    const uint8_t state;
    const uint8_t input;
    const uint8_t nextState;
    const uint8_t nextOutput;            

    bool operator == ( const StateTransition& object ) const;
    bool operator < ( const StateTransition& object ) const;
    bool operator > ( const StateTransition& object ) const;
    void print( void ) const;
};
#endif
