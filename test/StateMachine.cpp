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
  uint8_t index = transitionMatrix.find( state, input );
  if ( index != 0xFF ) {
    state = transitionMatrix.getNextState( index );
    output = transitionMatrix.getNextOutput( index );
    return 1;
  }
  index = transitionMatrix.find( state, DONT_CARE );
  if ( index != 0xFF ) {
    state = transitionMatrix.getNextState( index );
    output = transitionMatrix.getNextOutput( index );
    return 1;
  }
  return 0;
}


uint8_t StateMachine::getState( void ) {
  return state;
}


uint8_t StateMachine::getOutput( void ) {
  return output;
}
