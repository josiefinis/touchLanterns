#ifndef PRINT_TEXT_CPP
#define PRINT_TEXT_CPP

#include "Lantern.h"
#include "SensorInput.h"

#define START_TEST          0
#define END_TEST         0xFF
#define SET_SENSOR_VALUE    1
#define ASSERT_INPUT_EQ     2
#define ASSERT_STATE_EQ     3
#define ASSERT_OUPUT_EQ     4

#define PRINT_INPUT         1
#define PRINT_STATE         2
#define PRINT_OUTPUT        4
#define PRINT_BRIGHTNESS    8



struct Instruction {
  Instruction( int time, uint8_t instruction, uint16_t value=0, uint8_t idx=0 )
    : time( time )
    , instruction( instruction )
    , value( value )
    , idx( idx )
  {}
  int time;
  uint8_t instruction; 
  uint16_t value;
  uint8_t idx;
};


struct TestData {
  uint8_t input;
  uint8_t nextState;
  uint8_t nextOutput;
  uint8_t brightness;
};



//void printBrief( uint8_t input, uint8_t state, uint8_t output ) {
//  switch ( input ) {
//    case RISING_EDGE:                 std::cout << "+";      break;
//    case FALLING_EDGE:                std::cout << "-";      break;
//    case MEDIUM_TOUCH:                std::cout << "M";      break;
//    case LONG_TOUCH:                  std::cout << "L";      break;
//  }
//  switch ( state ) {
//    case IDLE:              std::cout << "ID";        
//    case GO_IDLE:           std::cout << "GI";        
//    case WAKE_DOWN:         std::cout << "in";        
//    case WAKE_UP:           std::cout << "IN";        
//    case FULL_DOWN:         std::cout << "fu";        
//    case FULL_UP:           std::cout << "FU";        
//    case FLICKER_DOWN:      std::cout << "fk";        
//    case FLICKER_UP:        std::cout << "FK";        
//    case AUTO_DOWN:         std::cout << "au";        
//    case AUTO_UP:           std::cout << "AU";        
//    case PAUSE_DOWN:        std::cout << "ps";        
//    case PAUSE_UP:          std::cout << "PS";        
//    case ROOT_FULL_DOWN:    std::cout << "rf";        
//    case ROOT_FULL_UP:      std::cout << "RF";        
//    case ROOT_FLICKER_DOWN: std::cout << "rk";        
//    case ROOT_FLICKER_UP:   std::cout << "RK";        
//    case ROOT_AUTO_DOWN:    std::cout << "ra";        
//    case ROOT_AUTO_UP:      std::cout << "RA";        
//    case ROOT_PAUSE_DOWN:   std::cout << "rp";        
//    case ROOT_PAUSE_UP:     std::cout << "RP";        
//    case WAIT:              std::cout << "W ";        
//    case GO_FULL_DOWN:      std::cout << "gf";        
//    case GO_FULL_UP:        std::cout << "gF";        
//    case GO_FOLLOW:         std::cout << "gF";        
//    case FOLLOW:            std::cout << "FW";        
//  }                                                   
//  switch ( output & 0xC0 ) {
//    case LOWER_BRIGHTNESS:          std::cout << "↓ ";      break;
//    case RAISE_BRIGHTNESS:          std::cout << "^ ";      break;
//    case SET_DELAY:                 std::cout << "D=";      break;       
//    default: break;
//  }
//  switch ( output ) {
//    case NO_CHANGE:                 std::cout << "  ";      break;       
//    case START_FLICKER:             std::cout << "SK";      break;   
//    case FLICKER:                   std::cout << "FK";      break;         
//    case PULSE:                     std::cout << "PU";      break;           
//    case SET_BRIGHTNESS_TO_ZERO:    std::cout << "B0";      break;
//    case SET_BRIGHTNESS_TO_FULL:    std::cout << "BF";      break;
//    case SET_BRIGHTNESS_TO_REF:     std::cout << "BR";      break;
//    case SET_REFERENCE_TO_ZERO:     std::cout << "R0";      break;
//    case SET_REF_TO_BRIGHTNESS:     std::cout << "RB";      break;
//    case TRACK_REFERENCE:           std::cout << "TR";      break; 
//    case TRACK_PARENT:              std::cout << "TP";      break; 
//    case MAKE_TREE:                 std::cout << "MT";      break;       
//    case LEAVE_TREE:                std::cout << "LT";      break;       
//    case REDUCE_DELAY:              std::cout << "D-";      break;    
//  }
//}


void printParent( uint8_t idx ) 
{
    if ( idx == 0xFF ) { std::cout << " "; }
    else       { std::cout << ( int ) idx; }
}                                                                      



void printInput( SensorInput input ) 
{
    if ( input == RISING_EDGE  ) { std::cout << "↑";  return; }
    if ( input == FALLING_EDGE ) { std::cout << "↓";  return; }
    if ( input == MEDIUM_TOUCH ) { std::cout << "M";  return; }
    if ( input == LONG_TOUCH   ) { std::cout << "L";  return; }
    std::cout << " ";
}                                                                      
                                                                       
                                                                       
void printRawInput( SensorInput input ) 
{
    std::cout << input;
}


void printExpectedInput( uint8_t input ) 
{
    if ( input == RISING_EDGE  ) { std::cout << "↑";  return; }
    if ( input == FALLING_EDGE ) { std::cout << "↓";  return; }
    if ( input == MEDIUM_TOUCH ) { std::cout << "M";  return; }
    if ( input == LONG_TOUCH   ) { std::cout << "L";  return; }
}                                                                      
                                                                       
                                                                       
void printState( uint8_t state ) {                                              
  switch ( state ) {                                                            
    case IDLE_ID:                   std::cout << "  ";        return;
    case WAKE_ID:                   std::cout << "W ";        return;
    case FULL_ID:                   std::cout << "F ";        return;
    case FLKR_ID:                   std::cout << "K ";        return;
    case AUTO_ID:                   std::cout << "A ";        return;
    case PAUS_ID:                   std::cout << "P ";        return;
  }
}

                                                                                
                                                                             
                                                                             
void printInstruction( Instruction instruction ) {                           
  switch ( instruction.instruction ) {
    case START_TEST:            
      std::cout << "START_TEST"; 
      break;
      
    case END_TEST:              
      std::cout << "END_TEST";   
      break;

    case SET_SENSOR_VALUE:             
      std::cout << "SET_INPUT " << ( int ) instruction.value;   
      break;

    case ASSERT_INPUT_EQ:
      std::cout << "ASSERT_INPUT_EQ  ";
      printExpectedInput( instruction.value );
      break;

    case ASSERT_STATE_EQ:
      std::cout << "ASSERT_STATE_EQ  ";
      printState( instruction.value );
      break;

//    case ASSERT_OUPUT_EQ:
//      std::cout << "ASSERT_OUPUT_EQ  ";
//      printOutput( instruction.value );
//      break;
  }
  std::cout << " at time " << instruction.time;
}
#endif
