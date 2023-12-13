/*
======================================================================================================================================================
                STATE TRANSITION
======================================================================================================================================================
*/

#include "StateTransition.h"


//constexpr StateTransition::StateTransition( const uint8_t state, const uint8_t input, const uint8_t nextState, const uint8_t nextOutput=NO_CHANGE ) {
//  this->state = state;
//  this->input = input;
//  this->nextState = nextState;
//  this->nextOutput = nextOutput;
//}


bool StateTransition::operator == ( const StateTransition& object ) const {
  return state == object.state and input == object.input;
}


bool StateTransition::operator < ( const StateTransition& object ) const {
  if ( state < object.state ) {
    return 1;
  }
  return state == object.state and input < object.input;
}


bool StateTransition::operator > ( const StateTransition& object ) const {
  if ( state > object.state ) {
    return 1;
  }
  return state == object.state and input > object.input;
}


void StateTransition::print( void ) const {
  std::cout << std::hex << "StateTransition : " << ( int ) state << ", " << ( int ) input << " --> " << ( int ) nextState << ", " << ( int ) nextOutput << std::dec << std::endl;
}
