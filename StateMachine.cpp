/*
======================================================================================================================================================
                STATE MACHINE
======================================================================================================================================================
*/

#ifndef STATE_MACHINE_CPP
#define STATE_MACHINE_CPP

#include "StateMachine.h"


StateMachine::StateMachine( const uint16_t& inputTable, const uint16_t& outputTable )
  : inputTable( inputTable )
  : outputTable( outputTable )
  , state( 0 )
  , output( 0 )
  {}


bool isInput( uint16_t input ) {
  index = transitionTable.find( state, input );       
  if ( index != NONE ) {
    return 1;
  }
  index = transitionTable.find( state, ALWAYS );       
  if ( index != NONE ) {
    return 1;
  }
  return 0;
}


uint16_t getOutput( void ) {
  return transitionTable.getOutput( index );
}


bool isStateTransition( void ) {
  return transitionTable.getNextState( index );
}


uint16_t getExitOutput( void ) {
  uint8_t index = transitionTable.find( state, ENTRY );
  if ( index != NONE ) {
    return transitionTable.getOutput( index + 1 ) | 1;
  }
  return 0;
}


void changeState( void ) {
  state = transitionTable.getNextState( index );
}


uint16_t getEntryOutput( void ) {
  uint8_t index = transitionTable.find( state, EXIT );
  if ( index != NONE ) {
    return transitionTable.getOutput( index ) | 1;
  }
  return 0;
}










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
#endif
