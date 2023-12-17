/*
======================================================================================================================================================
                STATE TRANSITION
======================================================================================================================================================
*/

#ifndef STATE_TRANSITION_CPP
#define STATE_TRANSITION_CPP

#include "StateTransition.h"


bool StateTransition::operator == ( const StateTransition& object ) const {
// Equal when both state and input are the same.
  return state == object.state and input == object.input;
}


bool StateTransition::operator < ( const StateTransition& object ) const {
// Less than: first compare state, then input.
  if ( state < object.state ) {
    return 1;
  }
  return state == object.state and input < object.input;
}


bool StateTransition::operator > ( const StateTransition& object ) const {
// Greater than: first compare state, then input.
  if ( state > object.state ) {
    return 1;
  }
  return state == object.state and input > object.input;
}
#endif
