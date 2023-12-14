#include "Lantern.h"
#include "Lantern.cpp"
#include "PrintText.cpp"


#define START_TEST          0
#define END_TEST         0xFF
#define SET_INPUT           1
#define ASSERT_INPUT_EQ     2
#define ASSERT_STATE_EQ     3
#define ASSERT_OUTPUT_EQ    4


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


void test_Lantern( void ) {
  Lantern lantern = Lantern();
}


void updateLantern( int t, Lantern& lantern, bool input, bool verbose=0 ) {
  lantern.pushInput( input );
  if ( verbose ) {
    std::cout << t << "\t" << ( int ) input << " T: "; 
    printState( lantern.getState() ); 
    std::cout << ",  "; 
    printInput( lantern.classifyInput() );
    std::cout << " ";
  }
  lantern.nextState();
  lantern.updateOutput();
  if ( verbose ) {
    std::cout << "---> ";
    printState( lantern.getState() ); 
    std::cout << ",  "; 
    printOutput( lantern.getOutput() );
  }
}


void updateBrightness( int t, Lantern& lantern, bool verbose=0 ) {
  for ( int j=0; j<16; j++ ) {
    lantern.changeBrightness();
    if ( verbose ) {
      std::cout << "\t" << ( int ) lantern.getBrightness();
    }
  }
  if ( verbose ) {
    std::cout << std::endl;
  }
}


bool assertInputEq( Lantern& lantern, uint8_t value , int t ) {
  if ( lantern.classifyInput() == value ) {
    return 1;
  }
  std::cout << std::endl << "Failed assertion at time " << t << ", input = "; 
  printInput( lantern.classifyInput() );
  std::cout << " (expected ";
  printInput( value );
  std::cout << ")." << std::endl << "\t\t\t\t\t";
  return 0;
}


bool assertStateEq( Lantern& lantern, uint8_t value , int t ) {
  if ( lantern.getState() == value ) {
    return 1;
  }
  std::cout << std::endl << "Failed assertion at time " << t << ", state = "; 
  printState( lantern.getState() );
  std::cout << " (expected ";
  printState( value );
  std::cout << ")." << std::endl << "\t\t\t\t\t";
  return 0;
}


bool assertOutputEq( Lantern& lantern, uint8_t value , int t ) {
  if ( lantern.getOutput() == value ) {
    return 1;
  }
  std::cout << std::endl << "Failed assertion at time " << t << ", output = "; 
  printOutput( lantern.getOutput() );
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

    updateLantern( t, lantern, input, verbose );

    while ( sequence[ i ].time == t ) {
      switch ( sequence[ i ].instruction ) {            // Perform instruction.
        case ASSERT_INPUT_EQ:
          if ( not assertInputEq( lantern, sequence[ i ].value , t ) ) { errorCount++; }
          break;

        case ASSERT_STATE_EQ:
          if ( not assertStateEq( lantern, sequence[ i ].value , t ) ) { errorCount++; }
          break;

        case ASSERT_OUTPUT_EQ:
          if ( not assertOutputEq( lantern, sequence[ i ].value , t ) ) { errorCount++; }
          break;
      }
      i++;
    }
    updateBrightness( t, lantern, verbose );
    if ( sequence[ i ].instruction == END_TEST ) {
      if ( not errorCount ) {
        std::cout << "Test complete. OK." << std::endl;
        return;
      }
      else {
        std::cout << "Test failed. " << errorCount << " errors." << std::endl;
        return;
      }
    }
  }
}


void test_FULL( bool verbose=0 ) {
  Lantern lantern = Lantern();
  std::cout << "Test FULL" << std::endl;
  Instruction sequence[] = {
      Instruction( 0    , START_TEST )
    , Instruction( 0    , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS )
    , Instruction( 0    , ASSERT_STATE_EQ, OUT )
    , Instruction( 0    , ASSERT_OUTPUT_EQ, NO_CHANGE )
    , Instruction( 8    , SET_INPUT, 1 )
    , Instruction( 8    , ASSERT_INPUT_EQ, RISING_EDGE )
    , Instruction( 8    , ASSERT_STATE_EQ, INIT_UP )
    , Instruction( 8    , ASSERT_OUTPUT_EQ, RAISE_BRIGHTNESS | 5 )
    , Instruction( 9    , SET_INPUT, 0 )
    , Instruction( 9    , ASSERT_INPUT_EQ, FALLING_EDGE )
    , Instruction( 9    , ASSERT_STATE_EQ, FULL_UP )
    , Instruction( 9    , ASSERT_OUTPUT_EQ, RAISE_BRIGHTNESS | 8 )
    , Instruction( 10   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS )
    , Instruction( 10   , ASSERT_STATE_EQ, GO_IDLE )
    , Instruction( 10   , ASSERT_OUTPUT_EQ, SET_BRIGHTNESS_TO_FULL )
    , Instruction( 12   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS )
    , Instruction( 12   , ASSERT_STATE_EQ, IDLE )
    , Instruction( 12   , ASSERT_OUTPUT_EQ, TRACK_REFERENCE )
    , Instruction( 20   , SET_INPUT, 1 )
    , Instruction( 20   , ASSERT_INPUT_EQ, RISING_EDGE )
    , Instruction( 20   , ASSERT_STATE_EQ, INIT_DOWN )
    , Instruction( 20   , ASSERT_OUTPUT_EQ, LOWER_BRIGHTNESS | 4 )
    , Instruction( 21   , SET_INPUT, 0 )
    , Instruction( 21   , ASSERT_INPUT_EQ, FALLING_EDGE )
    , Instruction( 21   , ASSERT_STATE_EQ, FULL_DOWN )
    , Instruction( 21   , ASSERT_OUTPUT_EQ, LOWER_BRIGHTNESS | 8 )
    , Instruction( 22   , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS )
    , Instruction( 22   , ASSERT_STATE_EQ, GO_OUT )
    , Instruction( 22   , ASSERT_OUTPUT_EQ, SET_BRIGHTNESS_TO_ZERO )
    , Instruction( 24   , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS )
    , Instruction( 24   , ASSERT_STATE_EQ, OUT )
    , Instruction( 24   , ASSERT_OUTPUT_EQ, NO_CHANGE )
    , Instruction( 32   , SET_INPUT, 1 )
    , Instruction( 32   , ASSERT_INPUT_EQ, RISING_EDGE )
    , Instruction( 32   , ASSERT_STATE_EQ, INIT_UP )
    , Instruction( 32   , ASSERT_OUTPUT_EQ, RAISE_BRIGHTNESS | 5 )
    , Instruction( 33   , SET_INPUT, 0 )
    , Instruction( 33   , ASSERT_INPUT_EQ, FALLING_EDGE )
    , Instruction( 33   , ASSERT_STATE_EQ, FULL_UP )
    , Instruction( 33   , ASSERT_OUTPUT_EQ, RAISE_BRIGHTNESS | 8 )
    , Instruction( 34   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS )
    , Instruction( 34   , ASSERT_STATE_EQ, GO_IDLE )
    , Instruction( 34   , ASSERT_OUTPUT_EQ, SET_BRIGHTNESS_TO_FULL )
    , Instruction( 36   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS )
    , Instruction( 36   , ASSERT_STATE_EQ, IDLE )
    , Instruction( 36   , ASSERT_OUTPUT_EQ, TRACK_REFERENCE )
    , Instruction( 44   , SET_INPUT, 1 )
    , Instruction( 44   , ASSERT_INPUT_EQ, RISING_EDGE )
    , Instruction( 44   , ASSERT_STATE_EQ, INIT_DOWN )
    , Instruction( 44   , ASSERT_OUTPUT_EQ, LOWER_BRIGHTNESS | 4 )
    , Instruction( 45   , SET_INPUT, 0 )
    , Instruction( 45   , ASSERT_INPUT_EQ, FALLING_EDGE )
    , Instruction( 45   , ASSERT_STATE_EQ, FULL_DOWN )
    , Instruction( 45   , ASSERT_OUTPUT_EQ, LOWER_BRIGHTNESS | 8 )
    , Instruction( 46   , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS )
    , Instruction( 46   , ASSERT_STATE_EQ, GO_OUT )
    , Instruction( 46   , ASSERT_OUTPUT_EQ, SET_BRIGHTNESS_TO_ZERO )
    , Instruction( 48   , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS )
    , Instruction( 48   , ASSERT_STATE_EQ, OUT )
    , Instruction( 48   , ASSERT_OUTPUT_EQ, NO_CHANGE )
    , Instruction( 60   , END_TEST )
  };
  runTestSequence( lantern, sequence, verbose );
}
  

void test_PAUSE( bool verbose=0 ) {
  Lantern lantern = Lantern();
  std::cout << "Test PAUSE" << std::endl;
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
  std::cout << "Test AUTO" << std::endl;
  Instruction testAuto[] = {
      Instruction( 0    , START_TEST )
    , Instruction( 0    , SET_INPUT, 0 )
    , Instruction( 0    , ASSERT_STATE_EQ, OUT )
    , Instruction( 11   , SET_INPUT, 1 )
    , Instruction( 12   , ASSERT_STATE_EQ, INIT_UP )
    , Instruction( 13   , ASSERT_STATE_EQ, INIT_UP )
    , Instruction( 15   , ASSERT_STATE_EQ, FLICKER_UP )
    , Instruction( 16   , ASSERT_STATE_EQ, FLICKER_UP )
    , Instruction( 16   , SET_INPUT, 0 )
    , Instruction( 17   , ASSERT_STATE_EQ, AUTO_UP )
    , Instruction( 96   , ASSERT_STATE_EQ, AUTO_UP )
    , Instruction( 98   , ASSERT_STATE_EQ, PAUSE_UP )
    , Instruction( 101  , ASSERT_STATE_EQ, PAUSE_UP )
    , Instruction( 107  , ASSERT_STATE_EQ, IDLE )
    , Instruction( 109  , SET_INPUT, 1 )
    , Instruction( 110  , ASSERT_STATE_EQ, INIT_DOWN )
    , Instruction( 112  , ASSERT_STATE_EQ, INIT_DOWN )
    , Instruction( 113  , ASSERT_STATE_EQ, FLICKER_DOWN )
    , Instruction( 115  , ASSERT_STATE_EQ, FLICKER_DOWN )
    , Instruction( 115  , SET_INPUT, 0 )
    , Instruction( 116  , ASSERT_STATE_EQ, AUTO_DOWN )
    , Instruction( 165  , SET_INPUT, 1 )
    , Instruction( 166  , SET_INPUT, 0 )
    , Instruction( 167  , ASSERT_STATE_EQ, PAUSE_DOWN )
    , Instruction( 175  , ASSERT_STATE_EQ, PAUSE_DOWN )
    , Instruction( 176  , ASSERT_STATE_EQ, IDLE )
    , Instruction( 189  , SET_INPUT, 1 )
    , Instruction( 190  , ASSERT_STATE_EQ, INIT_DOWN )
    , Instruction( 192  , ASSERT_STATE_EQ, INIT_DOWN )
    , Instruction( 193  , ASSERT_STATE_EQ, FLICKER_DOWN )
    , Instruction( 195  , ASSERT_STATE_EQ, FLICKER_DOWN )
    , Instruction( 195  , SET_INPUT, 0 )
    , Instruction( 196  , ASSERT_STATE_EQ, AUTO_DOWN )
    , Instruction( 196  , ASSERT_STATE_EQ, AUTO_DOWN )
    , Instruction( 270  , ASSERT_STATE_EQ, IDLE )
    , Instruction( 300  , END_TEST )
  };
  runTestSequence( lantern, testAuto, verbose );
}




int main( void ) {
  test_Lantern();
  test_FULL(1);
  //test_AUTO();
  //test_PAUSE();

  return 0;
}
