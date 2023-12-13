/*
======================================================================================================================================================
                STATE TRANSITION
======================================================================================================================================================
*/

constexpr StateTransition::StateTransition( const uint8_t state, const uint8_t input, const uint8_t nextState, const uint8_t nextOutput=NO_CHANGE ) {
  this->state = state;
  this->input = input;
  this->nextState = nextState;
  this->nextOutput = nextOutput;

}


bool StateTransition:: operator < ( const StateTransition& object ) {
  if ( this->state > object->state ) {
    return 0;
  }
  if ( this->input >= object->input ) {
    return 0;
  }
  return 1;
}


bool StateTransition::operator > ( const StateTransition& object ) {
  if ( this->state < object->state ) {
    return 0;
  }
  if ( this->input <= object->input ) {
    return 0;
  }
  return 1;
}
