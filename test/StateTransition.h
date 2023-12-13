/*
======================================================================================================================================================
                STATE TRANSITION
======================================================================================================================================================
*/

#ifndef STATE_TRANSITION_H
#define STATE_TRANSITION_H

#include <cstdint>
#include <iostream>


#define NO_CHANGE 0xFF


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
