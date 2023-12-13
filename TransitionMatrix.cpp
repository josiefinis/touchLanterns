/*
======================================================================================================================================================
                TRANSITION MATRIX
======================================================================================================================================================
*/

#include "TransititionMatrix.h"


static const StateTransition transitionMatrix[ TRANSISTION_MATRIX_ROWS ] = {


uint8_t TransitionMatrix::find( uint8_t state, uint8_t input ) {
// Find the state transition from 'state' on 'input' and return its row index in the transition matrix.
  StateTransition searchTerm, searchResult;
  searchTerm = { state, input };
  uint8_t start = 0;
  uint8_t end = TRANSITION_MATRIX_ROWS;
  while ( start != end ) {
    uint8_t searchIndex = ( end - start ) / 2;
    searchResult = { matrix[ searchIndex ].state, matrix[ searchIndex ].input };
    if ( searchResult == searchIndex ) {
      return searchIndex;
    }
    if ( searchTerm > searchResult ) {
      start = searchIndex;
    }
    else {
      end = searchIndex;
    }
  }
}


uint8_t TransitionMatrix::getNextState( uint8_t index ) {
  return matrix[ index ].nextState;
}


uint8_t TransitionMatrix::getNextOutput( uint8_t index ) {
  return matrix[ index ].nextOutput;
}
