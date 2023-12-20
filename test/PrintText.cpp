#ifndef PRINT_TEXT_CPP
#define PRINT_TEXT_CPP

#include "Lantern.h"

#define START_TEST          0
#define END_TEST         0xFF
#define SET_INPUT           1
#define ASSERT_INPUT_EQ     2
#define ASSERT_STATE_EQ     3
#define ASSERT_OUPUT_EQ     4

#define PRINT_INPUT         1
#define PRINT_STATE         2
#define PRINT_OUTPUT        4
#define PRINT_BRIGHTNESS    8



struct Instruction {
  Instruction( int time, uint8_t instruction, uint8_t value=0, uint8_t idx=0 )
    : time( time )
    , instruction( instruction )
    , value( value )
    , idx( idx )
  {}
  int time;
  uint8_t instruction; 
  uint8_t value;
  uint8_t idx;
};


struct TestData {
  uint8_t input;
  uint8_t nextState;
  uint8_t nextOutput;
  uint8_t brightness;
};



void printBrief( uint8_t input, uint8_t state, uint8_t output ) {
  switch ( input ) {
    case NO_INPUT:                    std::cout << " ";      break;
    case RISING_EDGE:                 std::cout << "+";      break;
    case FALLING_EDGE:                std::cout << "-";      break;
    case MEDIUM_TOUCH:                std::cout << "M";      break;
    case LONG_TOUCH:                  std::cout << "L";      break;
    case LONG_TOUCH_FALLING_EDGE:     std::cout << "l";      break;
    case AT_ZERO_BRIGHTNESS:          std::cout << "0";      break;
    case AT_ONE_BRIGHTNESS:           std::cout << "1";      break;
    case AT_FULL_BRIGHTNESS:          std::cout << "F";      break;
    case AT_ZERO_DELAY:               std::cout << "D";      break;
    case PARENT_IS_IDLE:              std::cout << "I";      break;
    case PARENT_IS_FULL_DOWN:         std::cout << "p";      break;
    case PARENT_IS_FULL_UP:           std::cout << "P";      break;
    case DONT_CARE:                   std::cout << "?";      break;
  }
  switch ( state ) {
    case OUT:               std::cout << "OU";        
    case GO_OUT:            std::cout << "GO";        
    case IDLE:              std::cout << "ID";        
    case GO_IDLE:           std::cout << "GI";        
    case INIT_DOWN:         std::cout << "in";        
    case INIT_UP:           std::cout << "IN";        
    case FULL_DOWN:         std::cout << "fu";        
    case FULL_UP:           std::cout << "FU";        
    case FLICKER_DOWN:      std::cout << "fk";        
    case FLICKER_UP:        std::cout << "FK";        
    case AUTO_DOWN:         std::cout << "au";        
    case AUTO_UP:           std::cout << "AU";        
    case PAUSE_DOWN:        std::cout << "ps";        
    case PAUSE_UP:          std::cout << "PS";        
    case ROOT_FULL_DOWN:    std::cout << "rf";        
    case ROOT_FULL_UP:      std::cout << "RF";        
    case ROOT_FLICKER_DOWN: std::cout << "rk";        
    case ROOT_FLICKER_UP:   std::cout << "RK";        
    case ROOT_AUTO_DOWN:    std::cout << "ra";        
    case ROOT_AUTO_UP:      std::cout << "RA";        
    case ROOT_PAUSE_DOWN:   std::cout << "rp";        
    case ROOT_PAUSE_UP:     std::cout << "RP";        
    case WAIT:              std::cout << "W ";        
    case GO_FULL_DOWN:      std::cout << "gf";        
    case GO_FULL_UP:        std::cout << "gF";        
    case GO_FOLLOW:         std::cout << "gF";        
    case FOLLOW:            std::cout << "FW";        
  }                                                   
  switch ( output & 0xC0 ) {
    case LOWER_BRIGHTNESS:          std::cout << "↓ ";      break;
    case RAISE_BRIGHTNESS:          std::cout << "^ ";      break;
    case SET_DELAY:                 std::cout << "D=";      break;       
    default: break;
  }
  switch ( output ) {
    case NO_CHANGE:                 std::cout << "  ";      break;       
    case START_FLICKER:             std::cout << "SK";      break;   
    case FLICKER:                   std::cout << "FK";      break;         
    case PULSE:                     std::cout << "PU";      break;           
    case SET_BRIGHTNESS_TO_ZERO:    std::cout << "B0";      break;
    case SET_BRIGHTNESS_TO_FULL:    std::cout << "BF";      break;
    case SET_BRIGHTNESS_TO_REF:     std::cout << "BR";      break;
    case SET_REFERENCE_TO_ZERO:     std::cout << "R0";      break;
    case SET_REF_TO_BRIGHTNESS:     std::cout << "RB";      break;
    case TRACK_REFERENCE:           std::cout << "TR";      break; 
    case TRACK_PARENT:              std::cout << "TP";      break; 
    case MAKE_TREE:                 std::cout << "MT";      break;       
    case LEAVE_TREE:                std::cout << "LT";      break;       
    case REDUCE_DELAY:              std::cout << "D-";      break;    
  }
}


void printInput( uint8_t input ) {
  switch ( input ) {
    case NO_INPUT:                    std::cout << "NO_I";      return;
    case RISING_EDGE:                 std::cout << "+EDG";      return;
    case FALLING_EDGE:                std::cout << "-EDG";      return;
    case MEDIUM_TOUCH:                std::cout << "MEDM";      return;
    case LONG_TOUCH:                  std::cout << "LONG";      return;
    case LONG_TOUCH_FALLING_EDGE:     std::cout << "LG-E";      return;
    case AT_ZERO_BRIGHTNESS:          std::cout << "@ZRO";      return;
    case AT_ONE_BRIGHTNESS:           std::cout << "@ONE";      return;
    case AT_FULL_BRIGHTNESS:          std::cout << "@FUL";      return;
    case AT_ZERO_DELAY:               std::cout << "@DLY";      return;
    case PARENT_IS_IDLE:              std::cout << "pIDL";      return;
    case PARENT_IS_FULL_DOWN:         std::cout << "pful";      return;
    case PARENT_IS_FULL_UP:           std::cout << "pFUL";      return;
    case PARENT_IS_FOLLOW:            std::cout << "pFOL";      return;
    case DONT_CARE:                   std::cout << "DTCR";      return;
  }                                                                    
}                                                                      
                                                                       
                                                                       
void printState( uint8_t state ) {                                              
  switch ( state ) {                                                            
    case OUT:               std::cout << " OUT";        return;
    case GO_OUT:            std::cout << ">OUT";        return;
    case IDLE:              std::cout << "IDLE";        return;
    case GO_IDLE:           std::cout << ">IDL";        return;
    case INIT_DOWN:         std::cout << "INIv";        return;
    case INIT_UP:           std::cout << "INI^";        return;
    case FULL_DOWN:         std::cout << "FULv";        return;
    case FULL_UP:           std::cout << "FUL^";        return;
    case FLICKER_DOWN:      std::cout << "FLIv";        return;
    case FLICKER_UP:        std::cout << "FLI^";        return;
    case AUTO_DOWN:         std::cout << "AUTv";        return;
    case AUTO_UP:           std::cout << "AUT^";        return;
    case PAUSE_DOWN:        std::cout << "PAUv";        return;
    case PAUSE_UP:          std::cout << "PAU^";        return;
    case ROOT_FULL_DOWN:    std::cout << "RFUv";        return;
    case ROOT_FULL_UP:      std::cout << "RFU^";        return;
    case ROOT_FLICKER_DOWN: std::cout << "RFLv";        return;
    case ROOT_FLICKER_UP:   std::cout << "RFL^";        return;
    case ROOT_AUTO_DOWN:    std::cout << "RAUv";        return;
    case ROOT_AUTO_UP:      std::cout << "RAU^";        return;
    case ROOT_PAUSE_DOWN:   std::cout << "RPAv";        return;
    case ROOT_PAUSE_UP:     std::cout << "RPA^";        return;
    case WAIT:              std::cout << "WAIT";        return;
    case GO_FULL_DOWN:      std::cout << "GOFD";        return;
    case GO_FULL_UP:        std::cout << "GOFU";        return;
    case GO_FOLLOW:         std::cout << "GOFO";        return;
    case FOLLOW:            std::cout << "FLLW";        return;
  }
}

                                                                                
void printOutput( uint8_t output ) {
  switch ( output & 0xC0 ) {
    case LOWER_BRIGHTNESS:          std::cout << "LOWER";      return;
    case RAISE_BRIGHTNESS:          std::cout << "RAISE";      return;
    case SET_DELAY:                 std::cout << "DLY =";      return;
  }                                                                   
  switch ( output ) {                                                 
    case NO_CHANGE:                 std::cout << "NO_CH";      return;
    case START_FLICKER:             std::cout << "S_FLK";      return;
    case FLICKER:                   std::cout << "FLIKR";      return;
    case PULSE:                     std::cout << "PULSE";      return;
    case SET_BRIGHTNESS_TO_ZERO:    std::cout << "B=ZRO";      return;
    case SET_BRIGHTNESS_TO_FULL:    std::cout << "B=FUL";      return;
    case SET_BRIGHTNESS_TO_REF:     std::cout << "B=REF";      return;
    case SET_REFERENCE_TO_ZERO:     std::cout << "REF=0";      return;
    case SET_REF_TO_BRIGHTNESS:     std::cout << "REF=B";      return;
    case TRACK_PARENT:              std::cout << "TR_PA";      return;
    case TRACK_REFERENCE:           std::cout << "TRACK";      return;
    case MAKE_TREE:                 std::cout << "TREE ";      return;
    case LEAVE_TREE:                std::cout << "LEAVE";      return;
    case REDUCE_DELAY:              std::cout << "DLY--";      return;
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

    case SET_INPUT:             
      std::cout << "SET_INPUT " << ( int ) instruction.value;   
      break;

    case ASSERT_INPUT_EQ:
      std::cout << "ASSERT_INPUT_EQ  ";
      printInput( instruction.value );
      break;

    case ASSERT_STATE_EQ:
      std::cout << "ASSERT_STATE_EQ  ";
      printState( instruction.value );
      break;

    case ASSERT_OUPUT_EQ:
      std::cout << "ASSERT_OUPUT_EQ  ";
      printOutput( instruction.value );
      break;
  }
  std::cout << " at time " << instruction.time;
}
#endif
