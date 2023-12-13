/*
======================================================================================================================================================
                STATE MACHINE
======================================================================================================================================================
*/

#include "StateMachine.h"
#include "TransitionMatrix.cpp"


StateMachine::StateMachine( const TransitionMatrix& matrix )
  : transitionMatrix( matrix )
  , state( 0 )
  , output( 0 )
  {}


bool StateMachine::next( uint8_t input ) {
// Update the state and output of StateMachine given the current state and new input.
  uint8_t index = transitionMatrix.find( state, input );        // First try to find row that matches state, input in TransitionMatrix.
  if ( index != 0xFF ) {
    state = transitionMatrix.getNextState( index );
    output = transitionMatrix.getNextOutput( index );
    return 1;
  }
  index = transitionMatrix.find( state, DONT_CARE );            // Then try to find row that matches state, where input is DONT_CARE.
  if ( index != 0xFF ) {
    state = transitionMatrix.getNextState( index );
    output = transitionMatrix.getNextOutput( index );
    return 1;
  }
  return 0;
}


uint8_t StateMachine::getState( void ) {
// Get the current state.
  return state;
}


uint8_t StateMachine::getOutput( void ) {
// Get the current output.
  return output;
}
