#include "LanternCollection.h"
#include "LanternCollection.cpp"


#include "PrintText.cpp"


const uint16_t neighbourList[16] = 
{ 
  0x421 , 0x4320, 0x5310, 0x521 ,
  0x7610, 0x7632, 0xC854, 0xC854,
  0xA976, 0xDBA8, 0xB98 , 0xFA9 ,
  0xED76, 0xFEC9, 0xFDC , 0xEDB 
};

LanternCollection collection = LanternCollection( 16, neighbourList );


TestData updateLantern( uint8_t idx, bool input ) {
  collection.pushSensor( idx, input );
  TestData preUpdate = collection.exportData( idx );
  collection.update( idx );
  TestData postUpdate = collection.exportData( idx );
  postUpdate.input = preUpdate.input;
  return postUpdate;
}


void updateBrightness( void ) {
  for ( uint8_t idx=0; idx<16; idx++ ) {
    collection.changeBrightness( idx );
  }
}


bool assertInputEq( uint8_t actual, uint8_t expected , int t ) {
  if ( actual == expected ) {
    return 1;
  }
  bool retVal = 0;
  if ( expected != DONT_CARE ) {
    std::cout << std::endl << "Failed assertion at time " << t << ", input = "; 
  }
  else {
    std::cout << std::endl << "Warning at time " << t << ", input = "; 
    retVal = 1;
  }
  printInput( actual );
  std::cout << " (expected ";
  printInput( expected );
  std::cout << ")." << std::endl << "\t\t\t\t\t";
  return retVal;
}


bool assertStateEq( uint8_t actual, uint8_t expected , int t ) {
  if ( actual == expected ) {
    return 1;
  }
  std::cout << std::endl << "Failed assertion at time " << t << ", state = "; 
  printState( actual );
  std::cout << " (expected ";
  printState( expected );
  std::cout << ")." << std::endl << "\t\t\t\t\t";
  return 0;
}


bool assertOutputEq( uint8_t actual, uint8_t expected , int t ) {
  if ( actual == expected ) {
    return 1;
  }
  std::cout << std::endl << "Failed assertion at time " << t << ", output = "; 
  printOutput( actual );
  std::cout << " (expected ";
  printOutput( expected );
  std::cout << ")." << std::endl << "\t\t\t\t\t";
  return 0;
}


void runTestSequence( Instruction sequence[], uint8_t verbose=0 ) {
  int errorCount = 0;
  uint8_t i = 0;
  int t = -1;
  bool input[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  while ( ++t < 301 ) {  
    if ( verbose ) { std::cout << std::endl << t << "\t"; }
    for ( uint8_t idx=0; idx<16; idx++ ) {
      if ( sequence[ i ].instruction == SET_INPUT and sequence[ i ].time == t and sequence[ i ].idx == idx ) { 
        input[ idx ] = sequence[ i ].value; 
      }
      TestData result = updateLantern( idx, input[ idx ] );
      if ( verbose == PRINT_INPUT ) {
        printInput( result.input );
      }
      if ( verbose == PRINT_STATE ) {
        printState( result.nextState );
      }
      if ( verbose == PRINT_OUTPUT ) {
        printOutput( result.nextOutput );
      }

      while ( sequence[ i ].time == t and sequence[ i ].idx == idx ) {
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
      updateBrightness();
      if ( verbose == PRINT_BRIGHTNESS ) {
        std::cout << std::dec << ( int ) collection.getBrightness( idx ); 
      }
      if ( verbose ) {
        std::cout << "\t";
      }
    }
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


void test_FULL( uint8_t verbose=0 ) {
  std::cout << "Test FULL";
  Instruction sequence[] = {
      Instruction( 0    , START_TEST )
    , Instruction( 0    , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS, 4 )
    , Instruction( 0    , ASSERT_STATE_EQ, OUT, 4 )
    , Instruction( 0    , ASSERT_OUPUT_EQ, NO_CHANGE, 4 )
    , Instruction( 8    , SET_INPUT, 1, 4 )
    , Instruction( 8    , ASSERT_INPUT_EQ, RISING_EDGE, 4 )
    , Instruction( 8    , ASSERT_STATE_EQ, INIT_UP, 4 )
    , Instruction( 8    , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 5, 4 )
    , Instruction( 9    , SET_INPUT, 0, 4 )
    , Instruction( 9    , ASSERT_INPUT_EQ, FALLING_EDGE, 4 )
    , Instruction( 9    , ASSERT_STATE_EQ, FULL_UP, 4 )
    , Instruction( 9    , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 8, 4 )
    , Instruction( 10   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS, 4 )
    , Instruction( 10   , ASSERT_STATE_EQ, GO_IDLE, 4 )
    , Instruction( 10   , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_FULL, 4 )
    , Instruction( 12   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS, 4 )
    , Instruction( 12   , ASSERT_STATE_EQ, IDLE, 4 )
    , Instruction( 12   , ASSERT_OUPUT_EQ, REDUCE_DELAY, 4 )
    , Instruction( 20   , SET_INPUT, 1, 4 )
    , Instruction( 20   , ASSERT_INPUT_EQ, RISING_EDGE, 4 )
    , Instruction( 20   , ASSERT_STATE_EQ, INIT_DOWN, 4 )
    , Instruction( 20   , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 4, 4 )
    , Instruction( 21   , SET_INPUT, 0, 4 )
    , Instruction( 21   , ASSERT_INPUT_EQ, FALLING_EDGE, 4 )
    , Instruction( 21   , ASSERT_STATE_EQ, FULL_DOWN, 4 )
    , Instruction( 21   , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 8, 4 )
    , Instruction( 22   , ASSERT_INPUT_EQ, AT_ONE_BRIGHTNESS, 4 )
    , Instruction( 22   , ASSERT_STATE_EQ, GO_OUT, 4 )
    , Instruction( 22   , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_ZERO, 4 )
    , Instruction( 24   , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS, 4 )
    , Instruction( 24   , ASSERT_STATE_EQ, OUT, 4 )
    , Instruction( 24   , ASSERT_OUPUT_EQ, NO_CHANGE, 4 )
    , Instruction( 32   , SET_INPUT, 1, 4 )
    , Instruction( 32   , ASSERT_INPUT_EQ, RISING_EDGE, 4 )
    , Instruction( 32   , ASSERT_STATE_EQ, INIT_UP, 4 )
    , Instruction( 32   , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 5, 4 )
    , Instruction( 33   , SET_INPUT, 0, 4 )
    , Instruction( 33   , ASSERT_INPUT_EQ, FALLING_EDGE, 4 )
    , Instruction( 33   , ASSERT_STATE_EQ, FULL_UP, 4 )
    , Instruction( 33   , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 8, 4 )
    , Instruction( 34   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS, 4 )
    , Instruction( 34   , ASSERT_STATE_EQ, GO_IDLE, 4 )
    , Instruction( 34   , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_FULL, 4 )
    , Instruction( 36   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS, 4 )
    , Instruction( 36   , ASSERT_STATE_EQ, IDLE, 4 )
    , Instruction( 36   , ASSERT_OUPUT_EQ, REDUCE_DELAY, 4 )
    , Instruction( 44   , SET_INPUT, 1, 4 )
    , Instruction( 44   , ASSERT_INPUT_EQ, RISING_EDGE, 4 )
    , Instruction( 44   , ASSERT_STATE_EQ, INIT_DOWN, 4 )
    , Instruction( 44   , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 4, 4 )
    , Instruction( 45   , SET_INPUT, 0, 4 )
    , Instruction( 45   , ASSERT_INPUT_EQ, FALLING_EDGE, 4 )
    , Instruction( 45   , ASSERT_STATE_EQ, FULL_DOWN, 4 )
    , Instruction( 45   , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 8, 4 )
    , Instruction( 46   , ASSERT_INPUT_EQ, AT_ONE_BRIGHTNESS, 4 )
    , Instruction( 46   , ASSERT_STATE_EQ, GO_OUT, 4 )
    , Instruction( 46   , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_ZERO, 4 )
    , Instruction( 48   , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS, 4 )
    , Instruction( 48   , ASSERT_STATE_EQ, OUT, 4 )
    , Instruction( 48   , ASSERT_OUPUT_EQ, NO_CHANGE, 4 )
    , Instruction( 60   , END_TEST )
  };
  runTestSequence( sequence, verbose );
}


void test_ROOT_FULL( uint8_t verbose=0 ) {
  std::cout << "Test ROOT_FULL";
  Instruction sequence[] = {
      Instruction( 0    , START_TEST )
    , Instruction( 0    , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS, 4 )
    , Instruction( 0    , ASSERT_STATE_EQ, OUT, 4 )
    , Instruction( 0    , ASSERT_OUPUT_EQ, NO_CHANGE, 4 )

    , Instruction( 8    , SET_INPUT, 1, 4 )
    , Instruction( 8    , ASSERT_INPUT_EQ, RISING_EDGE, 4 )
    , Instruction( 8    , ASSERT_STATE_EQ, INIT_UP, 4 )
    , Instruction( 8    , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 5, 4 )

    , Instruction( 9   , ASSERT_INPUT_EQ, DONT_CARE, 4 )
    , Instruction( 9   , ASSERT_STATE_EQ, INIT_UP, 4 )
    , Instruction( 9   , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 5, 4 )

    , Instruction( 11   , ASSERT_INPUT_EQ, MEDIUM_TOUCH, 4 )
    , Instruction( 11   , ASSERT_STATE_EQ, FLICKER_UP, 4 )
    , Instruction( 11   , ASSERT_OUPUT_EQ, START_FLICKER, 4 )

    , Instruction( 12   , ASSERT_INPUT_EQ, DONT_CARE, 4 )
    , Instruction( 12   , ASSERT_STATE_EQ, FLICKER_UP, 4 )
    , Instruction( 12   , ASSERT_OUPUT_EQ, FLICKER, 4 )

    , Instruction( 15   , ASSERT_INPUT_EQ, LONG_TOUCH, 4 )
    , Instruction( 15   , ASSERT_STATE_EQ, ROOT_FULL_UP, 4 )
    , Instruction( 15   , ASSERT_OUPUT_EQ, MAKE_TREE, 4 )

    , Instruction( 16   , SET_INPUT, 0, 4 )
    , Instruction( 16   , ASSERT_INPUT_EQ, LONG_TOUCH_FALLING_EDGE, 4 )
    , Instruction( 16   , ASSERT_STATE_EQ, FULL_UP, 4 )
    , Instruction( 16   , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 8, 4 )

    , Instruction( 17   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS, 4 )
    , Instruction( 17   , ASSERT_STATE_EQ, GO_IDLE, 4 )
    , Instruction( 17   , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_FULL, 4 )

    , Instruction( 18   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS, 4 )
    , Instruction( 18   , ASSERT_STATE_EQ, IDLE, 4 )
    , Instruction( 18   , ASSERT_OUPUT_EQ, SET_REF_TO_BRIGHTNESS, 4 )

    , Instruction( 21   , SET_INPUT, 1, 4 )
    , Instruction( 21   , ASSERT_INPUT_EQ, RISING_EDGE, 4 )
    , Instruction( 21   , ASSERT_STATE_EQ, INIT_DOWN, 4 )
    , Instruction( 21   , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 4, 4 )

    , Instruction( 22   , ASSERT_INPUT_EQ, DONT_CARE, 4 )
    , Instruction( 22   , ASSERT_STATE_EQ, INIT_DOWN, 4 )
    , Instruction( 22   , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 3, 4 )

    , Instruction( 24   , ASSERT_INPUT_EQ, MEDIUM_TOUCH, 4 )
    , Instruction( 24   , ASSERT_STATE_EQ, FLICKER_DOWN, 4 )
    , Instruction( 24   , ASSERT_OUPUT_EQ, START_FLICKER, 4 )

    , Instruction( 25   , ASSERT_INPUT_EQ, DONT_CARE, 4 )
    , Instruction( 25   , ASSERT_STATE_EQ, FLICKER_DOWN, 4 )
    , Instruction( 25   , ASSERT_OUPUT_EQ, FLICKER, 4 )

    , Instruction( 28   , ASSERT_INPUT_EQ, LONG_TOUCH, 4 )
    , Instruction( 28   , ASSERT_STATE_EQ, ROOT_FULL_DOWN, 4 )
    , Instruction( 28   , ASSERT_OUPUT_EQ, MAKE_TREE, 4 )

    , Instruction( 29   , SET_INPUT, 0, 4 )
    , Instruction( 29   , ASSERT_INPUT_EQ, LONG_TOUCH_FALLING_EDGE, 4 )
    , Instruction( 29   , ASSERT_STATE_EQ, FULL_DOWN, 4 )
    , Instruction( 29   , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 8, 4 )

    , Instruction( 30   , ASSERT_INPUT_EQ, AT_ONE_BRIGHTNESS, 4 )
    , Instruction( 30   , ASSERT_STATE_EQ, GO_OUT, 4 )
    , Instruction( 30   , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_ZERO, 4 )

    , Instruction( 31   , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS, 4 )
    , Instruction( 36   , ASSERT_STATE_EQ, OUT, 4 )
    , Instruction( 36   , ASSERT_OUPUT_EQ, NO_CHANGE, 4 )

    , Instruction( 50   , END_TEST )
  };
  runTestSequence( sequence, verbose );
}


void test_exportData( void ) {
  std::cout << "Test exportData." << std::endl;
  for ( uint8_t idx=0; idx<16; idx++ ) {
    TestData results = collection.exportData( idx );
    std::cout << std::hex << ( int ) idx << std::dec;
    printInput( results.input );
    printState( results.nextState );
    printOutput( results.nextOutput );
  }
  std::cout << std::endl;
  std::cout << ". OK." << std::endl;
}


void test_print( void ) {
  std::cout << "Test print." << std::endl;
  for( uint8_t idx=0; idx<16; idx++ ) {
    collection.print( idx );
  }
}


void test_pushSensor( void ) {
  std::cout << "Test pushSensor.";
  for ( int i=0; i<3000; i++ ) {
    for ( uint8_t idx=0; idx<16; idx++ ) {
      collection.pushSensor( idx, 1 );
    }
  }
  std::cout << ". OK." << std::endl;
}


void test_update( void ) {
  std::cout << "Test update.";
  collection.update( 0 );
  std::cout << ". OK." << std::endl;
}


void test_changeBrightness( void ) {
  std::cout << "Test changeBrightness.";
  for ( int i=0; i<3000; i++ ) {
    for ( uint8_t idx=0; idx<16; idx++ ) {
      collection.changeBrightness( idx );
    }
  }
  std::cout << ". OK." << std::endl;
}


void test_run( void ) {
  for ( uint8_t t=0; t<10; t++ ) {
    for ( uint8_t idx=0; idx<16; idx++ ) {
      collection.pushSensor( idx, 0 );
      if ( t == 0 and idx == 4 ) { collection.pushSensor( idx, 1 ); }
      collection.update( idx );
      updateBrightness();
      collection.print( idx );
    }
  }
}


int main( void ) {
  test_FULL();
  test_ROOT_FULL( 8 );

  return 0;
}
