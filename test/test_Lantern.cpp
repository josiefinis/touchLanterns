#include "Lantern.h"
#include "Lantern.cpp"


#define START_TEST          0
#define END_TEST         0xFF
#define SET_INPUT           1
#define ASSERT_INPUT_EQ     2
#define ASSERT_STATE_EQ     3
#define ASSERT_OUPUT_EQ     4


struct Instruction {
  Instruction( int time, uint8_t instruction, uint8_t value=0 )
    : time( time )
    , instruction( instruction )
    , value( value )
  {}
  int time;
  uint8_t instruction; 
  uint8_t value;
};

#include "PrintText.cpp"

struct Result {
  int time;
  uint8_t input;
  uint8_t nextState;
  uint8_t nextOutput;
};


void test_Lantern( void ) {
  Lantern lantern = Lantern();
}


Result updateLantern( int t, Lantern& lantern, bool input, bool verbose=0 ) {
  Result result;
  lantern.pushInput( input );
  result.input = lantern.classifyInput();
  if ( verbose ) {
    std::cout << std::endl << t << "\t" << ( int ) input << " T: "; 
    printState( lantern.getState() ); 
    std::cout << ",  "; 
    printInput( lantern.classifyInput() );
    std::cout << " ";
  }
  lantern.nextState();
  lantern.updateOutput();
  result.nextState = lantern.getState();
  result.nextOutput = lantern.getOutput();
  if ( verbose ) {
    std::cout << "---> ";
    printState( lantern.getState() ); 
    std::cout << ",  "; 
    printOutput( lantern.getOutput() );
    std::cout << "(" << ( int ) lantern.getDelay() << ")";
  }
  return result;
}


void updateBrightness( int t, Lantern& lantern, bool verbose=0 ) {
  for ( int j=0; j<16; j++ ) {
    lantern.changeBrightness();
    if ( verbose ) {
      std::cout << "\t" << ( int ) lantern.getBrightness();
    }
  }
}


bool assertInputEq( uint8_t result, uint8_t value , int t ) {
  if ( result == value ) {
    return 1;
  }
  bool retVal = 0;
  if ( value != DONT_CARE ) {
    std::cout << std::endl << "Failed assertion at time " << t << ", input = "; 
  }
  else {
    std::cout << std::endl << "Warning at time " << t << ", input = "; 
    retVal = 1;
  }
  printInput( result );
  std::cout << " (expected ";
  printInput( value );
  std::cout << ")." << std::endl << "\t\t\t\t\t";
  return retVal;
}


bool assertStateEq( uint8_t result, uint8_t value , int t ) {
  if ( result == value ) {
    return 1;
  }
  std::cout << std::endl << "Failed assertion at time " << t << ", state = "; 
  printState( result );
  std::cout << " (expected ";
  printState( value );
  std::cout << ")." << std::endl << "\t\t\t\t\t";
  return 0;
}


bool assertOutputEq( uint8_t result, uint8_t value , int t ) {
  if ( result == value ) {
    return 1;
  }
  std::cout << std::endl << "Failed assertion at time " << t << ", output = "; 
  printOutput( result );
  std::cout << " (expected ";
  printOutput( value );
  std::cout << ")." << std::endl << "\t\t\t\t\t";
  return 0;
}


void runTestSequence( Lantern& lantern, Instruction sequence[], bool verbose=0 ) {
  int errorCount = 0;
  uint8_t i = 0;
  int t = -1;
  bool input = 0;
  while ( ++t < 301 ) {  
    if ( sequence[ i ].time == t and sequence[ i ].instruction == SET_INPUT ) { 
      input = sequence[ i ].value; 
    }

    Result result = updateLantern( t, lantern, input, verbose );

    while ( sequence[ i ].time == t ) {
      switch ( sequence[ i ].instruction ) {            // Perform instruction.
        case ASSERT_INPUT_EQ:
          if ( not assertInputEq( result.input, sequence[ i ].value , t ) ) { errorCount++; }
          break;

        case ASSERT_STATE_EQ:
          if ( not assertStateEq( result.nextState, sequence[ i ].value , t ) ) { errorCount++; }
          break;

        case ASSERT_OUPUT_EQ:
          if ( not assertOutputEq( result.nextOutput, sequence[ i ].value , t ) ) { errorCount++; }
          break;
      }
      i++;
    }
    updateBrightness( t, lantern, verbose );
    if ( sequence[ i ].instruction == END_TEST ) {
      if ( verbose ) { 
        std::cout << std::endl;
      }
      if ( not errorCount ) {
        std::cout << ". OK." << std::endl;
        return;
      }
      else {
        std::cout << " Failed. " << errorCount << " errors." << std::endl;
        return;
      }
    }
  }
  std::cout << std::endl << std::endl << " ERROR. Timed out on instruction:  "; 
  printInstruction( sequence[i] );
  std::cout << std::endl<< std::endl;
}


void test_FULL( bool verbose=0 ) {
  Lantern lantern = Lantern();
  std::cout << "Test FULL";
  Instruction sequence[] = {
      Instruction( 0    , START_TEST )
    , Instruction( 0    , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS )
    , Instruction( 0    , ASSERT_STATE_EQ, OUT )
    , Instruction( 0    , ASSERT_OUPUT_EQ, NO_CHANGE )
    , Instruction( 8    , SET_INPUT, 1 )
    , Instruction( 8    , ASSERT_INPUT_EQ, RISING_EDGE )
    , Instruction( 8    , ASSERT_STATE_EQ, INIT_UP )
    , Instruction( 8    , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 5 )
    , Instruction( 9    , SET_INPUT, 0 )
    , Instruction( 9    , ASSERT_INPUT_EQ, FALLING_EDGE )
    , Instruction( 9    , ASSERT_STATE_EQ, FULL_UP )
    , Instruction( 9    , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 8 )
    , Instruction( 10   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS )
    , Instruction( 10   , ASSERT_STATE_EQ, GO_IDLE )
    , Instruction( 10   , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_FULL )
    , Instruction( 12   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS )
    , Instruction( 12   , ASSERT_STATE_EQ, IDLE )
    , Instruction( 12   , ASSERT_OUPUT_EQ, REDUCE_DELAY )
    , Instruction( 20   , SET_INPUT, 1 )
    , Instruction( 20   , ASSERT_INPUT_EQ, RISING_EDGE )
    , Instruction( 20   , ASSERT_STATE_EQ, INIT_DOWN )
    , Instruction( 20   , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 4 )
    , Instruction( 21   , SET_INPUT, 0 )
    , Instruction( 21   , ASSERT_INPUT_EQ, FALLING_EDGE )
    , Instruction( 21   , ASSERT_STATE_EQ, FULL_DOWN )
    , Instruction( 21   , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 8 )
    , Instruction( 22   , ASSERT_INPUT_EQ, AT_ONE_BRIGHTNESS )
    , Instruction( 22   , ASSERT_STATE_EQ, GO_OUT )
    , Instruction( 22   , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_ZERO )
    , Instruction( 24   , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS )
    , Instruction( 24   , ASSERT_STATE_EQ, OUT )
    , Instruction( 24   , ASSERT_OUPUT_EQ, NO_CHANGE )
    , Instruction( 32   , SET_INPUT, 1 )
    , Instruction( 32   , ASSERT_INPUT_EQ, RISING_EDGE )
    , Instruction( 32   , ASSERT_STATE_EQ, INIT_UP )
    , Instruction( 32   , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 5 )
    , Instruction( 33   , SET_INPUT, 0 )
    , Instruction( 33   , ASSERT_INPUT_EQ, FALLING_EDGE )
    , Instruction( 33   , ASSERT_STATE_EQ, FULL_UP )
    , Instruction( 33   , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 8 )
    , Instruction( 34   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS )
    , Instruction( 34   , ASSERT_STATE_EQ, GO_IDLE )
    , Instruction( 34   , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_FULL )
    , Instruction( 36   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS )
    , Instruction( 36   , ASSERT_STATE_EQ, IDLE )
    , Instruction( 36   , ASSERT_OUPUT_EQ, REDUCE_DELAY )
    , Instruction( 44   , SET_INPUT, 1 )
    , Instruction( 44   , ASSERT_INPUT_EQ, RISING_EDGE )
    , Instruction( 44   , ASSERT_STATE_EQ, INIT_DOWN )
    , Instruction( 44   , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 4 )
    , Instruction( 45   , SET_INPUT, 0 )
    , Instruction( 45   , ASSERT_INPUT_EQ, FALLING_EDGE )
    , Instruction( 45   , ASSERT_STATE_EQ, FULL_DOWN )
    , Instruction( 45   , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 8 )
    , Instruction( 46   , ASSERT_INPUT_EQ, AT_ONE_BRIGHTNESS )
    , Instruction( 46   , ASSERT_STATE_EQ, GO_OUT )
    , Instruction( 46   , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_ZERO )
    , Instruction( 48   , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS )
    , Instruction( 48   , ASSERT_STATE_EQ, OUT )
    , Instruction( 48   , ASSERT_OUPUT_EQ, NO_CHANGE )
    , Instruction( 60   , END_TEST )
  };
  runTestSequence( lantern, sequence, verbose );
}
  

void test_PAUSE( bool verbose=0 ) {
  Lantern lantern = Lantern();
  std::cout << "Test PAUSE";
  Instruction sequence[] = {
      Instruction( 0    , START_TEST )
    , Instruction( 0    , SET_INPUT, 0 )
    , Instruction( 0    , ASSERT_STATE_EQ, OUT )
    , Instruction( 8    , SET_INPUT, 1 )
    , Instruction( 12   , SET_INPUT, 0 )
    , Instruction( 13   , ASSERT_STATE_EQ, AUTO_UP )
    , Instruction( 30   , SET_INPUT, 1 )
    , Instruction( 31   , SET_INPUT, 0 )
    , Instruction( 32   , ASSERT_STATE_EQ, PAUSE_UP )
    , Instruction( 38   , SET_INPUT, 1 )
    , Instruction( 39   , SET_INPUT, 0 )
    , Instruction( 40   , ASSERT_STATE_EQ, AUTO_DOWN )
    , Instruction( 50   , SET_INPUT, 1 )
    , Instruction( 51   , SET_INPUT, 0 )
    , Instruction( 52   , ASSERT_STATE_EQ, PAUSE_DOWN )
    , Instruction( 58    , SET_INPUT, 1 )
    , Instruction( 62   , SET_INPUT, 0 )
    , Instruction( 63   , ASSERT_STATE_EQ, AUTO_UP )
    , Instruction( 80   , SET_INPUT, 1 )
    , Instruction( 81   , SET_INPUT, 0 )
    , Instruction( 82   , ASSERT_STATE_EQ, PAUSE_UP )
    , Instruction( 88   , SET_INPUT, 1 )
    , Instruction( 89   , SET_INPUT, 0 )
    , Instruction( 90   , ASSERT_STATE_EQ, AUTO_DOWN )
    , Instruction( 100  , SET_INPUT, 1 )
    , Instruction( 101  , SET_INPUT, 0 )
    , Instruction( 102  , ASSERT_STATE_EQ, PAUSE_DOWN )
    , Instruction( 120  , END_TEST )
  };
  runTestSequence( lantern, sequence, verbose );
}


void test_AUTO( bool verbose=0 ) {
  Lantern lantern = Lantern();
  std::cout << "Test AUTO";
  Instruction sequence[] = {
      Instruction( 0    , START_TEST )
    , Instruction( 0    , ASSERT_STATE_EQ, OUT )

    , Instruction( 11   , SET_INPUT, 1 )
    , Instruction( 11   , ASSERT_INPUT_EQ, RISING_EDGE )
    , Instruction( 11   , ASSERT_STATE_EQ, INIT_UP )
    , Instruction( 11   , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 5 )
    
    , Instruction( 13   , ASSERT_INPUT_EQ, DONT_CARE )
    , Instruction( 13   , ASSERT_STATE_EQ, INIT_UP )
    , Instruction( 13   , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 5 )

    , Instruction( 14   , ASSERT_INPUT_EQ, MEDIUM_TOUCH )
    , Instruction( 14   , ASSERT_STATE_EQ, FLICKER_UP )
    , Instruction( 14   , ASSERT_OUPUT_EQ, START_FLICKER )

    , Instruction( 15   , ASSERT_OUPUT_EQ, FLICKER )

    , Instruction( 16   , SET_INPUT, 0 )
    , Instruction( 16   , ASSERT_INPUT_EQ, FALLING_EDGE )
    , Instruction( 16   , ASSERT_STATE_EQ, AUTO_UP )
    , Instruction( 16   , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_REF )

    , Instruction( 17   , ASSERT_INPUT_EQ, DONT_CARE )
    , Instruction( 17   , ASSERT_STATE_EQ, AUTO_UP )
    , Instruction( 17   , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 1 )

    , Instruction( 96   , ASSERT_INPUT_EQ, DONT_CARE )
    , Instruction( 96   , ASSERT_STATE_EQ, AUTO_UP )
    , Instruction( 96   , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 1 )

    , Instruction( 97   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS )
    , Instruction( 97   , ASSERT_STATE_EQ, PAUSE_UP )
    , Instruction( 97   , ASSERT_OUPUT_EQ, SET_DELAY | 8 )

    , Instruction( 98   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS )
    , Instruction( 98   , ASSERT_STATE_EQ, PAUSE_UP )
    , Instruction( 98   , ASSERT_OUPUT_EQ, REDUCE_DELAY )

    , Instruction( 101  , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS )
    , Instruction( 101  , ASSERT_STATE_EQ, PAUSE_UP )
    , Instruction( 101  , ASSERT_OUPUT_EQ, REDUCE_DELAY )

    , Instruction( 106  , ASSERT_INPUT_EQ, AT_ZERO_DELAY )
    , Instruction( 106  , ASSERT_STATE_EQ, GO_IDLE )
    , Instruction( 106  , ASSERT_OUPUT_EQ, REDUCE_DELAY )

    , Instruction( 107  , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS )
    , Instruction( 107  , ASSERT_STATE_EQ, IDLE )
    , Instruction( 107  , ASSERT_OUPUT_EQ, SET_REF_TO_BRIGHTNESS )

    , Instruction( 121  , SET_INPUT, 1 )
    , Instruction( 121  , ASSERT_INPUT_EQ, RISING_EDGE )
    , Instruction( 121  , ASSERT_STATE_EQ, INIT_DOWN )
    , Instruction( 121  , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 4 )

    , Instruction( 123  , ASSERT_INPUT_EQ, DONT_CARE )
    , Instruction( 123  , ASSERT_STATE_EQ, INIT_DOWN )
    , Instruction( 123  , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 3 )

    , Instruction( 124  , ASSERT_INPUT_EQ, MEDIUM_TOUCH )
    , Instruction( 124  , ASSERT_STATE_EQ, FLICKER_DOWN )

    , Instruction( 125  , ASSERT_OUPUT_EQ, FLICKER )

    , Instruction( 126  , SET_INPUT, 0 )
    , Instruction( 126  , ASSERT_INPUT_EQ, FALLING_EDGE )
    , Instruction( 126  , ASSERT_STATE_EQ, AUTO_DOWN )
    , Instruction( 126  , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_REF )

    , Instruction( 127  , ASSERT_INPUT_EQ, DONT_CARE )
    , Instruction( 127  , ASSERT_STATE_EQ, AUTO_DOWN )
    , Instruction( 127  , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 1 )

    , Instruction( 237  , ASSERT_INPUT_EQ, DONT_CARE )
    , Instruction( 237  , ASSERT_STATE_EQ, AUTO_DOWN )
    , Instruction( 237  , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 1 )

    , Instruction( 238  , ASSERT_INPUT_EQ, AT_ONE_BRIGHTNESS )
    , Instruction( 238  , ASSERT_STATE_EQ, PAUSE_DOWN )
    , Instruction( 238  , ASSERT_OUPUT_EQ, SET_DELAY | 8 )

    , Instruction( 239  , ASSERT_INPUT_EQ, AT_ONE_BRIGHTNESS )
    , Instruction( 239  , ASSERT_STATE_EQ, PAUSE_DOWN )
    , Instruction( 239  , ASSERT_OUPUT_EQ, REDUCE_DELAY )

    , Instruction( 242  , ASSERT_INPUT_EQ, AT_ONE_BRIGHTNESS )
    , Instruction( 242  , ASSERT_STATE_EQ, PAUSE_DOWN )
    , Instruction( 242  , ASSERT_OUPUT_EQ, REDUCE_DELAY )

    , Instruction( 247  , ASSERT_INPUT_EQ, AT_ZERO_DELAY )
    , Instruction( 247  , ASSERT_STATE_EQ, GO_IDLE )
    , Instruction( 247  , ASSERT_OUPUT_EQ, REDUCE_DELAY )

    , Instruction( 248  , ASSERT_INPUT_EQ, AT_ONE_BRIGHTNESS )
    , Instruction( 248  , ASSERT_STATE_EQ, IDLE )
    , Instruction( 248  , ASSERT_OUPUT_EQ, SET_REF_TO_BRIGHTNESS )

    , Instruction( 251  , END_TEST )
  };
  runTestSequence( lantern, sequence, verbose );
}


void test_dontLetGo( bool verbose=0 ) {
  Lantern lantern = Lantern();
  std::cout << "Test don't let go";
  Instruction sequence[] = {
      Instruction( 0    , START_TEST )
    , Instruction( 0    , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS )
    , Instruction( 0    , ASSERT_STATE_EQ, OUT )
    , Instruction( 0    , ASSERT_OUPUT_EQ, NO_CHANGE )

    , Instruction( 8    , SET_INPUT, 1 )
    , Instruction( 8    , ASSERT_INPUT_EQ, RISING_EDGE )
    , Instruction( 8    , ASSERT_STATE_EQ, INIT_UP )
    , Instruction( 8    , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 5 )

    , Instruction( 9    , ASSERT_INPUT_EQ, DONT_CARE )
    , Instruction( 9    , ASSERT_STATE_EQ, INIT_UP )
    , Instruction( 9    , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 5 )

    , Instruction( 11   , ASSERT_INPUT_EQ, MEDIUM_TOUCH )
    , Instruction( 11   , ASSERT_STATE_EQ, FLICKER_UP )
    , Instruction( 11   , ASSERT_OUPUT_EQ, START_FLICKER )

    , Instruction( 12   , ASSERT_INPUT_EQ, DONT_CARE )
    , Instruction( 12   , ASSERT_STATE_EQ, FLICKER_UP )
    , Instruction( 12   , ASSERT_OUPUT_EQ, FLICKER )

    , Instruction( 15   , ASSERT_INPUT_EQ, LONG_TOUCH )
    , Instruction( 15   , ASSERT_STATE_EQ, ROOT_FULL_UP )
    , Instruction( 15   , ASSERT_OUPUT_EQ, MAKE_TREE )

    , Instruction( 16   , ASSERT_INPUT_EQ, LONG_TOUCH )
    , Instruction( 16   , ASSERT_STATE_EQ, ROOT_FULL_UP )
    , Instruction( 16   , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 8 )

    , Instruction( 49   , ASSERT_INPUT_EQ, LONG_TOUCH )
    , Instruction( 49   , ASSERT_STATE_EQ, ROOT_FULL_UP )
    , Instruction( 49   , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 8 )

    , Instruction( 50   , END_TEST )
  };
  runTestSequence( lantern, sequence, verbose );
}


void test_ROOT_FULL( bool verbose=0 ) {
  Lantern lantern = Lantern();
  std::cout << "Test ROOT_FULL";
  Instruction sequence[] = {
      Instruction( 0    , START_TEST )
    , Instruction( 0    , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS )
    , Instruction( 0    , ASSERT_STATE_EQ, OUT )
    , Instruction( 0    , ASSERT_OUPUT_EQ, NO_CHANGE )

    , Instruction( 8    , SET_INPUT, 1 )
    , Instruction( 8    , ASSERT_INPUT_EQ, RISING_EDGE )
    , Instruction( 8    , ASSERT_STATE_EQ, INIT_UP )
    , Instruction( 8    , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 5 )

    , Instruction( 9   , ASSERT_INPUT_EQ, DONT_CARE )
    , Instruction( 9   , ASSERT_STATE_EQ, INIT_UP )
    , Instruction( 9   , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 5 )

    , Instruction( 11   , ASSERT_INPUT_EQ, MEDIUM_TOUCH )
    , Instruction( 11   , ASSERT_STATE_EQ, FLICKER_UP )
    , Instruction( 11   , ASSERT_OUPUT_EQ, START_FLICKER )

    , Instruction( 12   , ASSERT_INPUT_EQ, DONT_CARE )
    , Instruction( 12   , ASSERT_STATE_EQ, FLICKER_UP )
    , Instruction( 12   , ASSERT_OUPUT_EQ, FLICKER )

    , Instruction( 15   , ASSERT_INPUT_EQ, LONG_TOUCH )
    , Instruction( 15   , ASSERT_STATE_EQ, ROOT_FULL_UP )
    , Instruction( 15   , ASSERT_OUPUT_EQ, MAKE_TREE )

    , Instruction( 16   , SET_INPUT, 0 )
    , Instruction( 16   , ASSERT_INPUT_EQ, LONG_TOUCH_FALLING_EDGE )
    , Instruction( 16   , ASSERT_STATE_EQ, FULL_UP )
    , Instruction( 16   , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 8 )

    , Instruction( 17   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS )
    , Instruction( 17   , ASSERT_STATE_EQ, GO_IDLE )
    , Instruction( 17   , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_FULL )

    , Instruction( 18   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS )
    , Instruction( 18   , ASSERT_STATE_EQ, IDLE )
    , Instruction( 18   , ASSERT_OUPUT_EQ, SET_REF_TO_BRIGHTNESS )

    , Instruction( 21   , SET_INPUT, 1 )
    , Instruction( 21   , ASSERT_INPUT_EQ, RISING_EDGE )
    , Instruction( 21   , ASSERT_STATE_EQ, INIT_DOWN )
    , Instruction( 21   , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 4 )

    , Instruction( 22   , ASSERT_INPUT_EQ, DONT_CARE )
    , Instruction( 22   , ASSERT_STATE_EQ, INIT_DOWN )
    , Instruction( 22   , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 3 )

    , Instruction( 24   , ASSERT_INPUT_EQ, MEDIUM_TOUCH )
    , Instruction( 24   , ASSERT_STATE_EQ, FLICKER_DOWN )
    , Instruction( 24   , ASSERT_OUPUT_EQ, START_FLICKER )

    , Instruction( 25   , ASSERT_INPUT_EQ, DONT_CARE )
    , Instruction( 25   , ASSERT_STATE_EQ, FLICKER_DOWN )
    , Instruction( 25   , ASSERT_OUPUT_EQ, FLICKER )

    , Instruction( 28   , ASSERT_INPUT_EQ, LONG_TOUCH )
    , Instruction( 28   , ASSERT_STATE_EQ, ROOT_FULL_DOWN )
    , Instruction( 28   , ASSERT_OUPUT_EQ, MAKE_TREE )

    , Instruction( 29   , SET_INPUT, 0 )
    , Instruction( 29   , ASSERT_INPUT_EQ, LONG_TOUCH_FALLING_EDGE )
    , Instruction( 29   , ASSERT_STATE_EQ, FULL_DOWN )
    , Instruction( 29   , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 8 )

    , Instruction( 30   , ASSERT_INPUT_EQ, AT_ONE_BRIGHTNESS )
    , Instruction( 30   , ASSERT_STATE_EQ, GO_OUT )
    , Instruction( 30   , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_ZERO )

    , Instruction( 31   , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS )
    , Instruction( 36   , ASSERT_STATE_EQ, OUT )
    , Instruction( 36   , ASSERT_OUPUT_EQ, NO_CHANGE )

    , Instruction( 50   , END_TEST )
  };
  runTestSequence( lantern, sequence, verbose );
}


int main( void ) {
  test_Lantern();
  test_FULL();
  test_AUTO();
  test_PAUSE();
  test_dontLetGo();
  test_ROOT_FULL();

  return 0;
}
