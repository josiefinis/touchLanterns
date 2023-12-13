/*
======================================================================================================================================================
                TRANSITION MATRIX
======================================================================================================================================================
*/

#include "TransitionMatrix.h"
#include "StateTransition.cpp"
#include <iostream>


TransitionMatrix::TransitionMatrix( const uint8_t rows, const StateTransition* matrix ) 
  : rows( rows ), matrix( matrix ) {}
   

uint8_t TransitionMatrix::find( const uint8_t state, const uint8_t input ) const {
// Find the state transition from 'state' on 'input' and return its row index in the transition matrix.
  StateTransition searchTerm = { state, input };
  int lowerBound = 0;
  int upperBound = rows - 1;
  while ( lowerBound <= upperBound ) {
    uint8_t searchIndex = ( upperBound + lowerBound ) / 2;
    StateTransition searchResult = matrix[ searchIndex ];
    if ( searchResult == searchTerm ) {
      return searchIndex;
    }
    if ( searchTerm > searchResult ) {
      lowerBound = searchIndex + 1;
      continue;
    }
    if ( searchTerm < searchResult ) {
      upperBound = searchIndex - 1;
      continue;
    }
  }
  return NOT_FOUND;
}


uint8_t TransitionMatrix::getNextState( uint8_t index ) const {
  if ( index >= rows ) {
    return NOT_FOUND;
  }
  return matrix[ index ].nextState;
}


uint8_t TransitionMatrix::getNextOutput( uint8_t index ) const {
  if ( index >= rows ) {
    return NOT_FOUND;
  }
  return matrix[ index ].nextOutput;
}
