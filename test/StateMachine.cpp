/*
======================================================================================================================================================
                STATE MACHINE
======================================================================================================================================================
*/

#include "StateMachine.h"
#include "TransitionMatrix.cpp"


StateMachine::StateMachine( const TransitionMatrix& matrix )
  : transitionMatrix( matrix ), state( 0 ), input( 0 ), output( 0 )
  {}


void StateMachine::pushInput( bool value ) {
  input <<= 1;
  input |= value;
}


bool StateMachine::next( void ) {
  uint8_t index = transitionMatrix.find( state, input );
  if ( index == 0xFF ) {
    return 0;
  }
  state = transitionMatrix.getNextState( index );
  output = transitionMatrix.getNextOutput( index );
  return 1;
}


void StateMachine::setInput( uint8_t value ) {
  input = value;
}


uint8_t StateMachine::getState( void ) {
  return state;
}


uint8_t StateMachine::getInput( void ) {
  return input;
}


uint8_t StateMachine::getOutput( void ) {
  return output;
}
