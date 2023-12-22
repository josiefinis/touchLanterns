/*
======================================================================================================================================================
                TRANSITION TABLE
======================================================================================================================================================
*/

#ifndef TRANSITION_TABLE_CPP
#define TRANSITION_TABLE_CPP

#include "TransitionTable.h"


TransitionTable::TransitionTable( const uint8_t rows, const uint32_t* transistionTable ) 
  : rows( rows )
  , transition( transitionTable ) 
{ }
   

uint8_t TransitionTable::find( const uint8_t state, const uint16_t input ) const {
// Find the input condition T( state, input ) and return its row index.
  uint32_t searchTerm = state << 28 | input << 16;
  int lowerBound = 0;
  int upperBound = rows - 1;
  while ( lowerBound <= upperBound ) {
    uint8_t searchIndex = ( upperBound + lowerBound ) / 2;
    //uint16_t searchResult = inputTable[ searchIndex ];
    uint32_t = pgm_read_dword_near( transitionTable + searchIndex );
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


uint8_t TransitionTable::getNextState( uint8_t index ) const {
// Get nextState of indexed 
  if ( index >= rows ) {
    return NOT_FOUND;
  }
  return pgm_read_dword_near( transitionTable  + index ) & OUTPUT_MASK;
}


uint16_t TransitionTable::getOutput( uint8_t index ) const {
// Get output of indexed 
  if ( index >= rows ) {
    return NOT_FOUND;
  }
  return pgm_read_word_near(  transitionTable + index ) & OUTPUT_MASK;
}


bool TransitionTable::checkTable( void ) {
// Check table is sorted in ascending order.
  for ( uint8_t i=1; i<rows; i++ ) {
    if ( table[ i ] < table[ i - 1] ) {
      return 0;
    }
  }
  return 1;
}
#endif
