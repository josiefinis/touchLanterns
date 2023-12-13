/*
======================================================================================================================================================
                STATE TRANSITION
======================================================================================================================================================
*/

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


void StateTransition::print( void ) const {
// Print state transition T: state, input --> nextState, nextOutput.
  std::cout << std::hex << "StateTransition : " << ( int ) state << ", " << ( int ) input << " --> " << ( int ) nextState << ", " << ( int ) nextOutput << std::dec << std::endl;
}
