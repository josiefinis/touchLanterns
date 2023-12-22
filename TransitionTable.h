/*
======================================================================================================================================================
                TRANSITION TABLE
======================================================================================================================================================

An array where each row defines a state transition for a given input,

  T: state, input --> nextState, output.
  row: 4-bit state, 12-bit input, 4-bit nextState, 12-bit output.

          Sub table for each state:
            state   0x000   0x0         entry output            // Input's most significant bit is 0 for entry and exit outputs.
            state   0x000   0x0         exit output
            state   input   0x0         output                  // Rows of input outputs and state transitions in input order.
            state   input   nextState   output                  // Input's most significant bit is 1 for input conditions
            state   input   nextState   output                  // Output's most significant bit is 0 for no output, otherwise 1.
            state   input   0x0         output
            ...     ...     ...         ...

MUST BE SORTED.
*/

#ifndef TRANSITION_TABLE_H
#define TRANSITION_TABLE_H

#include "Arduino.h"

#define NOT_FOUND       0xffff;
#define STATE_MASK      0x0000000f;
#define OUTPUT_MASK     0x0000fff0;

class TransitionTable {
  public:
    TransitionTable( const uint8_t rows, const uint32_t* transitionTable );

    uint8_t find( uint8_t const state, const uint16_t input ) const;
    uint8_t getNextState( uint8_t index ) const;
    uint8_t getOutput( uint8_t index ) const;
    bool checkTable( void );

  private:
    const uint8_t rows;
    const uint32_t* transitionTable;
  
};
#endif
