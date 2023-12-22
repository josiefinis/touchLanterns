/*
======================================================================================================================================================
                STATE MACHINE
======================================================================================================================================================
                                      
*/

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "Arduino.h"

#define ALWAYS      0x00
#define ENTRY       0x01
#define EXIT        0x02
#define NONE        0xff

class StateMachine {
  public:
    StateMachine( const uint16_t& inputTable, const uint16_t& outputTable );


bool isInput( uint16_t input );
uint16_t getOutput( void );
bool isStateTransition( void );
uint16_t getExitOutput( void );
void changeState( void );
uint16_t getEntryOutput( void );


  private:
    const uint16_t& inputTable;
    const uint16_t& outputTable;
    uint8_t tableIndex;
    uint8_t state;
    uint8_t nextState;
};
#endif

/*

State machine logic

                                      input
                                        |
                                        ↓
                  ------------> [ wait for input ]
                 |                      | 
                 |                      ↓
                 |                 < isInput? > ----yes-----> [ getOutput ]
                 |                      |                           |
                 |                      |                           |
                 |                      no <------------------------                                     
                 |                      |
                 |                      ↓
                 no---------  < isStateTransition? > ------yes-----> [ getExitOutput ]
                 ^                                                           |
                 |                                                           |
                 |                                                           ↓
                 |------------- [ getEntryOutput ] <---------------- [ change state ]



    if ( isInput( input ) ) {
      output = getOutput();
      // Carry out output actions.
    }
    if ( isStateTransition() ) {
      output = getExitOutput();
      // Carry out output actions.
      changeState();
      output = getEntryOutput()
      // Carry out output actions.
    }
*/
