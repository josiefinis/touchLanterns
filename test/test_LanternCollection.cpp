#include "Global.h"
#include "ArduinoShadows.cpp"
#include "../LanternCollection.h"
#include "../LanternCollection.cpp"
#include "../Bridge.cpp"
#include "../Lantern.cpp"
#include "../Light.cpp"
#include "../SensorInput.cpp"
#include "../Tree.cpp"
#include "../State.cpp"
#include "../Graph.cpp"
#include "../Random.cpp"
#include "../Queue.cpp"
#include "../ShuffledList.cpp"
#include "PrintText.cpp"

#define NONE    0xff


const uint16_t neighbourList[16] = 
{ 
  0x421 , 0x4320, 0x5310, 0x521 ,
  0x7610, 0x7632, 0xC854, 0xC854,
  0xA976, 0xDBA8, 0xB98 , 0xFA9 ,
  0xED76, 0xFEC9, 0xFDC , 0xEDB 
};


LanternCollection collection = LanternCollection( 16, neighbourList );
Bridge bridge;




bool assertInputEq( SensorInput actual, uint8_t expected , int t ) {
    if ( actual == expected ) {
        return 1;
    }
    std::cout << std::endl << "Failed assertion at time " << t << "(";
    std::cout << " (expected ";
    printExpectedInput( expected );
    std::cout << ")." << std::endl << "\t\t\t\t\t";
    return 0;
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
//
//
//bool assertOutputEq( uint8_t actual, uint8_t expected , int t ) {
//  if ( actual == expected ) {
//    return 1;
//  }
//  std::cout << std::endl << "Failed assertion at time " << t << ", output = "; 
//  printOutput( actual );
//  std::cout << " (expected ";
//  printOutput( expected );
//  std::cout << ")." << std::endl << "\t\t\t\t\t";
//  return 0;
//}
//
//
void runTestSequence( Instruction sequence[], uint8_t verbose=0 ) 
{
    int errorCount = 0;
    uint8_t insIdx = 0;
    int t = -1;
    uint16_t input = 0;
    while ( ++t < 0x501 ) 
    {  
        if ( verbose ) { 
            int halfSecs = millis() >> 9;
            std::cout << std::endl << std::hex << t; 
            std::cout << "\t" << halfSecs << "\t";
        }

        int        time = sequence[ insIdx ].time;
        int         idx = sequence[ insIdx ].idx;
        int instruction = sequence[ insIdx ].instruction;
        int       value = sequence[ insIdx ].value;


        if ( instruction == END_TEST and t == time ) 
        {
            if ( verbose ) { std::cout << std::endl; }
            if ( not errorCount ) 
            {
                std::cout << ". OK." << std::endl;
                return;
            }
            else 
            {
                std::cout << " Failed. " << errorCount << " errors." << std::endl;
                return;
            }
        }
        if ( instruction == SET_SENSOR_VALUE and time == t and idx == t%16 ) 
        { 
            input = value << idx; 
        }
        for ( uint8_t idx=0; idx<16; idx++ )
        {
            if ( t%16 == idx ) { collection.updateCollection( idx, input >> idx & 1 ); }
            else { collection.updateCollection( idx ); }
            if ( verbose )
            {
                Lantern& lantern = collection[ idx ];
                uint8_t delay = bridge.getDelay( lantern );
                uint8_t parent = bridge.getParent( collection, lantern );
                std::cout << "\t"; 
                printInput( bridge.getInput( lantern ) );
                printParent( parent );
                printState( bridge.getState( lantern ) );
                if ( parent != NONE and delay  )
                {
                    std::cout << ":" << ( int ) bridge.getDelay( lantern );
                    int pauseUntil = millis() + 10;
                    while ( millis() < pauseUntil ) { }
                }
                else
                {
                    if ( collection.getBrightness( idx ) ) { std::cout << ( int ) collection.getBrightness( idx ); }
                }
            }
        }
        while ( time == t )
        {
            Lantern& lantern = collection[ idx ];
            switch ( instruction ) 
            {   // Perform instruction.
                case ASSERT_INPUT_EQ:
                    if ( not assertInputEq( bridge.getInput( lantern ), value , t ) ) { errorCount++; }
                    break;
  
                case ASSERT_STATE_EQ:
                    if ( not assertStateEq( bridge.getState( lantern ), value , t ) ) { errorCount++; }
                    break;
            }
            insIdx++;
            time = sequence[ insIdx ].time;
            idx = sequence[ insIdx ].idx;
            instruction = sequence[ insIdx ].instruction;
            value = sequence[ insIdx ].value;
        }
        if ( verbose ) 
        {
            std::cout << "\t";
        }
    }
    std::cout << std::endl << std::endl << " ERROR. Timed out on instruction:  "; 
    printInstruction( sequence[ insIdx] );
    std::cout << std::endl<< std::endl;
}
//
//
//void test_FULL( uint8_t verbose=0 ) {
//  std::cout << "Test FULL";
//  Instruction sequence[] = {
//      Instruction( 0    , START_TEST )
//    , Instruction( 0    , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS, 4 )
//    , Instruction( 0    , ASSERT_STATE_EQ, OUT, 4 )
//    , Instruction( 0    , ASSERT_OUPUT_EQ, NO_CHANGE, 4 )
//    , Instruction( 8    , SET_SENSOR_VALUE, 1, 4 )
//    , Instruction( 8    , ASSERT_INPUT_EQ, RISING_EDGE, 4 )
//    , Instruction( 8    , ASSERT_STATE_EQ, INIT_UP, 4 )
//    , Instruction( 8    , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 5, 4 )
//    , Instruction( 9    , SET_SENSOR_VALUE, 0, 4 )
//    , Instruction( 9    , ASSERT_INPUT_EQ, FALLING_EDGE, 4 )
//    , Instruction( 9    , ASSERT_STATE_EQ, FULL_UP, 4 )
//    , Instruction( 9    , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 8, 4 )
//    , Instruction( 10   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS, 4 )
//    , Instruction( 10   , ASSERT_STATE_EQ, GO_IDLE, 4 )
//    , Instruction( 10   , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_FULL, 4 )
//    , Instruction( 12   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS, 4 )
//    , Instruction( 12   , ASSERT_STATE_EQ, IDLE, 4 )
//    , Instruction( 12   , ASSERT_OUPUT_EQ, REDUCE_DELAY, 4 )
//    , Instruction( 20   , SET_SENSOR_VALUE, 1, 4 )
//    , Instruction( 20   , ASSERT_INPUT_EQ, RISING_EDGE, 4 )
//    , Instruction( 20   , ASSERT_STATE_EQ, INIT_DOWN, 4 )
//    , Instruction( 20   , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 4, 4 )
//    , Instruction( 21   , SET_SENSOR_VALUE, 0, 4 )
//    , Instruction( 21   , ASSERT_INPUT_EQ, FALLING_EDGE, 4 )
//    , Instruction( 21   , ASSERT_STATE_EQ, FULL_DOWN, 4 )
//    , Instruction( 21   , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 8, 4 )
//    , Instruction( 22   , ASSERT_INPUT_EQ, AT_ONE_BRIGHTNESS, 4 )
//    , Instruction( 22   , ASSERT_STATE_EQ, GO_OUT, 4 )
//    , Instruction( 22   , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_ZERO, 4 )
//    , Instruction( 24   , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS, 4 )
//    , Instruction( 24   , ASSERT_STATE_EQ, OUT, 4 )
//    , Instruction( 24   , ASSERT_OUPUT_EQ, NO_CHANGE, 4 )
//    , Instruction( 32   , SET_SENSOR_VALUE, 1, 4 )
//    , Instruction( 32   , ASSERT_INPUT_EQ, RISING_EDGE, 4 )
//    , Instruction( 32   , ASSERT_STATE_EQ, INIT_UP, 4 )
//    , Instruction( 32   , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 5, 4 )
//    , Instruction( 33   , SET_SENSOR_VALUE, 0, 4 )
//    , Instruction( 33   , ASSERT_INPUT_EQ, FALLING_EDGE, 4 )
//    , Instruction( 33   , ASSERT_STATE_EQ, FULL_UP, 4 )
//    , Instruction( 33   , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 8, 4 )
//    , Instruction( 34   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS, 4 )
//    , Instruction( 34   , ASSERT_STATE_EQ, GO_IDLE, 4 )
//    , Instruction( 34   , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_FULL, 4 )
//    , Instruction( 36   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS, 4 )
//    , Instruction( 36   , ASSERT_STATE_EQ, IDLE, 4 )
//    , Instruction( 36   , ASSERT_OUPUT_EQ, REDUCE_DELAY, 4 )
//    , Instruction( 44   , SET_SENSOR_VALUE, 1, 4 )
//    , Instruction( 44   , ASSERT_INPUT_EQ, RISING_EDGE, 4 )
//    , Instruction( 44   , ASSERT_STATE_EQ, INIT_DOWN, 4 )
//    , Instruction( 44   , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 4, 4 )
//    , Instruction( 45   , SET_SENSOR_VALUE, 0, 4 )
//    , Instruction( 45   , ASSERT_INPUT_EQ, FALLING_EDGE, 4 )
//    , Instruction( 45   , ASSERT_STATE_EQ, FULL_DOWN, 4 )
//    , Instruction( 45   , ASSERT_OUPUT_EQ, LOWER_BRIGHTNESS | 8, 4 )
//    , Instruction( 46   , ASSERT_INPUT_EQ, AT_ONE_BRIGHTNESS, 4 )
//    , Instruction( 46   , ASSERT_STATE_EQ, GO_OUT, 4 )
//    , Instruction( 46   , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_ZERO, 4 )
//    , Instruction( 48   , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS, 4 )
//    , Instruction( 48   , ASSERT_STATE_EQ, OUT, 4 )
//    , Instruction( 48   , ASSERT_OUPUT_EQ, NO_CHANGE, 4 )
//    , Instruction( 60   , END_TEST )
//  };
//  runTestSequence( sequence, verbose );
//}
//
//
//void test_ROOT_FULL( uint8_t verbose=0 ) {
//  std::cout << "Test ROOT_FULL";
//  Instruction sequence[] = {
//      Instruction( 0    , START_TEST )
//    , Instruction( 0    , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS, 4 )
//    , Instruction( 0    , ASSERT_STATE_EQ, OUT, 4 )
//    , Instruction( 0    , ASSERT_OUPUT_EQ, NO_CHANGE, 4 )
//
//    , Instruction( 8    , SET_SENSOR_VALUE, 1, 4 )
//    , Instruction( 8    , ASSERT_INPUT_EQ, RISING_EDGE, 4 )
//    , Instruction( 8    , ASSERT_STATE_EQ, INIT_UP, 4 )
//    , Instruction( 8    , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 5, 4 )
//
//    , Instruction( 9   , ASSERT_INPUT_EQ, DONT_CARE, 4 )
//    , Instruction( 9   , ASSERT_STATE_EQ, INIT_UP, 4 )
//    , Instruction( 9   , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 5, 4 )
//
//    , Instruction( 11   , ASSERT_INPUT_EQ, MEDIUM_TOUCH, 4 )
//    , Instruction( 11   , ASSERT_STATE_EQ, FLICKER_UP, 4 )
//    , Instruction( 11   , ASSERT_OUPUT_EQ, START_FLICKER, 4 )
//
//    , Instruction( 12   , ASSERT_INPUT_EQ, DONT_CARE, 4 )
//    , Instruction( 12   , ASSERT_STATE_EQ, FLICKER_UP, 4 )
//    , Instruction( 12   , ASSERT_OUPUT_EQ, FLICKER, 4 )
//
//    , Instruction( 15   , ASSERT_INPUT_EQ, LONG_TOUCH, 4 )
//    , Instruction( 15   , ASSERT_STATE_EQ, ROOT_FULL_UP, 4 )
//    , Instruction( 15   , ASSERT_OUPUT_EQ, MAKE_TREE, 4 )
//
//    , Instruction( 16   , SET_SENSOR_VALUE, 0, 4 )
//    , Instruction( 16   , ASSERT_INPUT_EQ, LONG_TOUCH_FALLING_EDGE, 4 )
//    , Instruction( 16   , ASSERT_STATE_EQ, FULL_UP, 4 )
//    , Instruction( 16   , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 8, 4 )
//
//    , Instruction( 17   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS, 4 )
//    , Instruction( 17   , ASSERT_STATE_EQ, GO_IDLE, 4 )
//    , Instruction( 17   , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_FULL, 4 )
//
//    , Instruction( 18   , ASSERT_INPUT_EQ, AT_FULL_BRIGHTNESS, 4 )
//    , Instruction( 18   , ASSERT_STATE_EQ, IDLE, 4 )
//    , Instruction( 18   , ASSERT_OUPUT_EQ, SET_REF_TO_BRIGHTNESS, 4 )
//
//
//    , Instruction( 999  , ASSERT_OUPUT_EQ, NO_CHANGE, 4 )
//    , Instruction( 1000  , END_TEST )
//  };
//  runTestSequence( sequence, verbose );
//}
//
//
//void test_AUTO( uint8_t verbose=0 ) {
//  std::cout << "Test AUTO";
//  Instruction sequence[] = {
//      Instruction( 0    , START_TEST )
//    , Instruction( 0    , ASSERT_INPUT_EQ, AT_ZERO_BRIGHTNESS, 4 )
//    , Instruction( 0    , ASSERT_STATE_EQ, OUT, 4 )
//    , Instruction( 0    , ASSERT_OUPUT_EQ, NO_CHANGE, 4 )
//
//    , Instruction( 8    , SET_SENSOR_VALUE, 1, 4 )
//    , Instruction( 8    , ASSERT_INPUT_EQ, RISING_EDGE, 4 )
//    , Instruction( 8    , ASSERT_STATE_EQ, INIT_UP, 4 )
//    , Instruction( 8    , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 5, 4 )
//
//    , Instruction( 9   , ASSERT_INPUT_EQ, DONT_CARE, 4 )
//    , Instruction( 9   , ASSERT_STATE_EQ, INIT_UP, 4 )
//    , Instruction( 9   , ASSERT_OUPUT_EQ, RAISE_BRIGHTNESS | 5, 4 )
//
//    , Instruction( 11   , ASSERT_INPUT_EQ, MEDIUM_TOUCH, 4 )
//    , Instruction( 11   , ASSERT_STATE_EQ, FLICKER_UP, 4 )
//    , Instruction( 11   , ASSERT_OUPUT_EQ, START_FLICKER, 4 )
//
//    , Instruction( 12   , ASSERT_INPUT_EQ, DONT_CARE, 4 )
//    , Instruction( 12   , ASSERT_STATE_EQ, FLICKER_UP, 4 )
//    , Instruction( 12   , ASSERT_OUPUT_EQ, FLICKER, 4 )
//
//    , Instruction( 14   , SET_SENSOR_VALUE, 0, 4 )
//    , Instruction( 14   , ASSERT_INPUT_EQ, FALLING_EDGE, 4 )
//    , Instruction( 14   , ASSERT_STATE_EQ, AUTO_UP, 4 )
//    , Instruction( 14   , ASSERT_OUPUT_EQ, SET_BRIGHTNESS_TO_REF, 4 )
//
//    , Instruction( 49   , ASSERT_STATE_EQ, IDLE, 4 )
//
//    , Instruction( 50   , END_TEST )
//  };
//  runTestSequence( sequence, verbose );
//}
//
//

void test_LONG( uint8_t verbose=0 ) {
    std::cout << "Test LONG";
    Instruction sequence[] = {
          Instruction( 0x00    , START_TEST )
        , Instruction( 0x00    , ASSERT_STATE_EQ, IDLE_ID, 4 )

        , Instruction( 0x04    , SET_SENSOR_VALUE, 1, 4 )
        , Instruction( 0x04    , ASSERT_INPUT_EQ, RISING_EDGE, 4 )
        , Instruction( 0x04    , ASSERT_STATE_EQ, WAKE_ID, 4 )

        , Instruction( 0x34   , ASSERT_INPUT_EQ, MEDIUM_TOUCH, 4 )
        , Instruction( 0x34   , ASSERT_STATE_EQ, FLKR_ID, 4 )

        , Instruction( 0x74   , ASSERT_INPUT_EQ, LONG_TOUCH, 4 )
        , Instruction( 0x74   , ASSERT_STATE_EQ, FULL_ID, 4 )

        , Instruction( 0x84   , SET_SENSOR_VALUE, 0, 4 )
        , Instruction( 0x84   , ASSERT_INPUT_EQ, FALLING_EDGE, 4 )

        , Instruction( 0x94   , ASSERT_STATE_EQ, IDLE_ID, 4 )
        , Instruction( 0x2f1   , END_TEST )
    };
    runTestSequence( sequence, verbose );
}

void test_FULL( uint8_t verbose=0 ) {
    std::cout << "Test FULL";
    Instruction sequence[] = {
          Instruction( 0x00    , START_TEST )
        , Instruction( 0x00    , ASSERT_STATE_EQ, IDLE_ID, 4 )

        , Instruction( 0x14    , SET_SENSOR_VALUE, 1, 4 )
        , Instruction( 0x14    , ASSERT_INPUT_EQ, RISING_EDGE, 4 )
        , Instruction( 0x14    , ASSERT_STATE_EQ, WAKE_ID, 4 )

        , Instruction( 0x24   , SET_SENSOR_VALUE, 0, 4 )
        , Instruction( 0x24   , ASSERT_INPUT_EQ, FALLING_EDGE, 4 )
        , Instruction( 0x24   , ASSERT_STATE_EQ, FULL_ID, 4 )

        , Instruction( 0x44   , ASSERT_STATE_EQ, IDLE_ID, 4 )
        , Instruction( 0x50   , END_TEST )
    };
    runTestSequence( sequence, verbose );
}


void test_rootAUTO( uint8_t verbose=0 ) {
    std::cout << "Test root AUTO";
    Instruction sequence[] = {
          Instruction( 0x00    , START_TEST )
        , Instruction( 0x00    , ASSERT_STATE_EQ, IDLE_ID, 4 )

        , Instruction( 0x04    , SET_SENSOR_VALUE, 1, 4 )
        , Instruction( 0x04    , ASSERT_INPUT_EQ, RISING_EDGE, 4 )
        , Instruction( 0x04    , ASSERT_STATE_EQ, WAKE_ID, 4 )

        , Instruction( 0x34    , ASSERT_INPUT_EQ, MEDIUM_TOUCH, 4 )
        , Instruction( 0x34    , ASSERT_STATE_EQ, FLKR_ID, 4 )

        , Instruction( 0x44    , SET_SENSOR_VALUE, 0, 4 )
        , Instruction( 0x44    , ASSERT_INPUT_EQ, FALLING_EDGE, 4 )
        , Instruction( 0x44    , ASSERT_STATE_EQ, AUTO_ID, 4 )

        , Instruction( 0x74    , SET_SENSOR_VALUE, 1, 4 )
        , Instruction( 0x74    , ASSERT_INPUT_EQ, RISING_EDGE, 4 )
        , Instruction( 0x74    , ASSERT_STATE_EQ, PAUS_ID, 4 )

        , Instruction( 0xa4    , ASSERT_INPUT_EQ, MEDIUM_TOUCH, 4 )
        , Instruction( 0xa4    , ASSERT_STATE_EQ, RIPL_ID, 4 )

        , Instruction( 0xb4    , SET_SENSOR_VALUE, 0, 4 )
        , Instruction( 0xb4    , ASSERT_INPUT_EQ, FALLING_EDGE, 4 )
        , Instruction( 0xb4    , ASSERT_STATE_EQ, RIPL_ID, 4 )

        , Instruction( 0x144   , ASSERT_STATE_EQ, AUTO_ID, 4 )

        , Instruction( 0x154   , SET_SENSOR_VALUE, 1, 4 )
        , Instruction( 0x154   , ASSERT_INPUT_EQ, RISING_EDGE, 4 )
        , Instruction( 0x154   , ASSERT_STATE_EQ, PAUS_ID, 4 )

        , Instruction( 0x164   , SET_SENSOR_VALUE, 0, 4 )
        , Instruction( 0x164   , ASSERT_INPUT_EQ, FALLING_EDGE, 4 )
        , Instruction( 0x164   , ASSERT_STATE_EQ, PAUS_ID, 4 )

        , Instruction( 0x3f1   , END_TEST )
    };
    runTestSequence( sequence, verbose );
}


int main( void ) {
    //test_FULL( 1 );
    //test_LONG( 1 );
    test_rootAUTO( 1 );
    //test_AUTO( 1 );

  return 0;
}
