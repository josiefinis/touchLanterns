#include "Lantern.h"

void printInput( uint8_t input ) {
  switch ( input ) {
    case NO_INPUT:                    std::cout << "NO_I";      return;
    case RISING_EDGE:                 std::cout << "+EDG";      return;
    case FALLING_EDGE:                std::cout << "-EDG";      return;
    case MEDIUM_TOUCH:                std::cout << "MEDM";      return;
    case LONG_TOUCH:                  std::cout << "LONG";      return;
    case LONG_TOUCH_FALLING_EDGE:     std::cout << "LG-E";      return;
    case LONG_RELEASE:                std::cout << "LG_R";      return;
    case AT_ZERO_BRIGHTNESS:          std::cout << "@ZRO";      return;
    case AT_ONE_BRIGHTNESS:           std::cout << "@ONE";      return;
    case AT_FULL_BRIGHTNESS:          std::cout << "@FUL";      return;
    case AT_ZERO_DELAY:               std::cout << "@DLY";      return;
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
    case WAIT_FULL_DOWN:    std::cout << "WT v";        return;
    case WAIT_FULL_UP:      std::cout << "WT ^";        return;
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
    case TRACK_REFERENCE:           std::cout << "TRACK";      return; 
    case MAKE_TREE:                 std::cout << "TREE ";      return;       
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
