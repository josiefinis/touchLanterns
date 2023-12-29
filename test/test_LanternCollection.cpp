#include "Global.h"
#include "ArduinoFunctions.cpp"
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
    uint16_t insIdx = 0;
    int t = -1;
    uint16_t input = 0;
    while ( ++t < 0xfff ) 
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
        if ( instruction == SET_SENSOR_VALUE and time == t and idx == t%16 ) { input = value << idx; }
        if ( instruction == BURN_DOWN        and time == t ) 
        { 
            for ( int i=0; i<16; ++i ) { collection.burnDown( i ); }
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
//                    if ( bridge.getReference( lantern ) )      { std::cout << ( int ) bridge.getReference( lantern ); }
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

void test_burnDown( uint8_t verbose=0 ) {
    std::cout << "Test burnDown";
    Instruction sequence[] = {
          Instruction( 0x00    , START_TEST )
        , Instruction( 0x00    , ASSERT_STATE_EQ, IDLE_ID, 4 )

        , Instruction( 0x04    , SET_SENSOR_VALUE, 1, 4 )
        , Instruction( 0x04    , ASSERT_INPUT_EQ, RISING_EDGE, 4 )
        , Instruction( 0x04    , ASSERT_STATE_EQ, WAKE_ID, 4 )

        , Instruction( 0x34    , ASSERT_INPUT_EQ, MEDIUM_TOUCH, 4 )
        , Instruction( 0x34    , ASSERT_STATE_EQ, FLKR_ID, 4 )

        , Instruction( 0x94    , SET_SENSOR_VALUE, 0, 4 )
        , Instruction( 0x94    , ASSERT_INPUT_EQ, FALLING_EDGE, 4 )
        , Instruction( 0x94    , ASSERT_STATE_EQ, FULL_ID, 4 )

        , Instruction( 0x117    , BURN_DOWN )
        , Instruction( 0x118    , BURN_DOWN )
        , Instruction( 0x119    , BURN_DOWN )
        , Instruction( 0x11a    , BURN_DOWN )
        , Instruction( 0x11b    , BURN_DOWN )
        , Instruction( 0x11c    , BURN_DOWN )
        , Instruction( 0x11d    , BURN_DOWN )
        , Instruction( 0x11e    , BURN_DOWN )
        , Instruction( 0x11f    , BURN_DOWN )
        , Instruction( 0x120    , BURN_DOWN )
        , Instruction( 0x121    , BURN_DOWN )
        , Instruction( 0x122    , BURN_DOWN )
        , Instruction( 0x123    , BURN_DOWN )
        , Instruction( 0x124    , BURN_DOWN )
        , Instruction( 0x125    , BURN_DOWN )
        , Instruction( 0x126    , BURN_DOWN )
        , Instruction( 0x127    , BURN_DOWN )
        , Instruction( 0x128    , BURN_DOWN )
        , Instruction( 0x129    , BURN_DOWN )
        , Instruction( 0x12a    , BURN_DOWN )
        , Instruction( 0x12b    , BURN_DOWN )
        , Instruction( 0x12c    , BURN_DOWN )
        , Instruction( 0x12d    , BURN_DOWN )
        , Instruction( 0x12e    , BURN_DOWN )
        , Instruction( 0x12f    , BURN_DOWN )
        , Instruction( 0x130    , BURN_DOWN )
        , Instruction( 0x131    , BURN_DOWN )
        , Instruction( 0x132    , BURN_DOWN )
        , Instruction( 0x133    , BURN_DOWN )
        , Instruction( 0x134    , BURN_DOWN )
        , Instruction( 0x135    , BURN_DOWN )
        , Instruction( 0x136    , BURN_DOWN )
        , Instruction( 0x137    , BURN_DOWN )
        , Instruction( 0x138    , BURN_DOWN )
        , Instruction( 0x139    , BURN_DOWN )
        , Instruction( 0x13a    , BURN_DOWN )
        , Instruction( 0x13b    , BURN_DOWN )
        , Instruction( 0x13c    , BURN_DOWN )
        , Instruction( 0x13d    , BURN_DOWN )
        , Instruction( 0x13e    , BURN_DOWN )
        , Instruction( 0x13f    , BURN_DOWN )
        , Instruction( 0x140    , BURN_DOWN )
        , Instruction( 0x141    , BURN_DOWN )
        , Instruction( 0x142    , BURN_DOWN )
        , Instruction( 0x143    , BURN_DOWN )
        , Instruction( 0x144    , BURN_DOWN )
        , Instruction( 0x145    , BURN_DOWN )
        , Instruction( 0x146    , BURN_DOWN )
        , Instruction( 0x147    , BURN_DOWN )
        , Instruction( 0x148    , BURN_DOWN )
        , Instruction( 0x149    , BURN_DOWN )
        , Instruction( 0x14a    , BURN_DOWN )
        , Instruction( 0x14b    , BURN_DOWN )
        , Instruction( 0x14c    , BURN_DOWN )
        , Instruction( 0x14d    , BURN_DOWN )
        , Instruction( 0x14e    , BURN_DOWN )
        , Instruction( 0x14f    , BURN_DOWN )
        , Instruction( 0x150    , BURN_DOWN )
        , Instruction( 0x151    , BURN_DOWN )
        , Instruction( 0x152    , BURN_DOWN )
        , Instruction( 0x153    , BURN_DOWN )
        , Instruction( 0x154    , BURN_DOWN )
        , Instruction( 0x155    , BURN_DOWN )
        , Instruction( 0x156    , BURN_DOWN )
        , Instruction( 0x157    , BURN_DOWN )
        , Instruction( 0x158    , BURN_DOWN )
        , Instruction( 0x159    , BURN_DOWN )
        , Instruction( 0x15a    , BURN_DOWN )
        , Instruction( 0x15b    , BURN_DOWN )
        , Instruction( 0x15c    , BURN_DOWN )
        , Instruction( 0x15d    , BURN_DOWN )
        , Instruction( 0x15e    , BURN_DOWN )
        , Instruction( 0x15f    , BURN_DOWN )
        , Instruction( 0x160    , BURN_DOWN )
        , Instruction( 0x161    , BURN_DOWN )
        , Instruction( 0x162    , BURN_DOWN )
        , Instruction( 0x163    , BURN_DOWN )
        , Instruction( 0x164    , BURN_DOWN )
        , Instruction( 0x165    , BURN_DOWN )
        , Instruction( 0x166    , BURN_DOWN )
        , Instruction( 0x167    , BURN_DOWN )
        , Instruction( 0x168    , BURN_DOWN )
        , Instruction( 0x169    , BURN_DOWN )
        , Instruction( 0x16a    , BURN_DOWN )
        , Instruction( 0x16b    , BURN_DOWN )
        , Instruction( 0x16c    , BURN_DOWN )
        , Instruction( 0x16d    , BURN_DOWN )
        , Instruction( 0x16e    , BURN_DOWN )
        , Instruction( 0x16f    , BURN_DOWN )
        , Instruction( 0x170    , BURN_DOWN )
        , Instruction( 0x171    , BURN_DOWN )
        , Instruction( 0x172    , BURN_DOWN )
        , Instruction( 0x173    , BURN_DOWN )
        , Instruction( 0x174    , BURN_DOWN )
        , Instruction( 0x175    , BURN_DOWN )
        , Instruction( 0x176    , BURN_DOWN )
        , Instruction( 0x177    , BURN_DOWN )
        , Instruction( 0x178    , BURN_DOWN )
        , Instruction( 0x179    , BURN_DOWN )
        , Instruction( 0x17a    , BURN_DOWN )
        , Instruction( 0x17b    , BURN_DOWN )
        , Instruction( 0x17c    , BURN_DOWN )
        , Instruction( 0x17d    , BURN_DOWN )
        , Instruction( 0x17e    , BURN_DOWN )
        , Instruction( 0x17f    , BURN_DOWN )
        , Instruction( 0x180    , BURN_DOWN )
        , Instruction( 0x181    , BURN_DOWN )
        , Instruction( 0x182    , BURN_DOWN )
        , Instruction( 0x183    , BURN_DOWN )
        , Instruction( 0x184    , BURN_DOWN )
        , Instruction( 0x185    , BURN_DOWN )
        , Instruction( 0x186    , BURN_DOWN )
        , Instruction( 0x187    , BURN_DOWN )
        , Instruction( 0x188    , BURN_DOWN )
        , Instruction( 0x189    , BURN_DOWN )
        , Instruction( 0x18a    , BURN_DOWN )
        , Instruction( 0x18b    , BURN_DOWN )
        , Instruction( 0x18c    , BURN_DOWN )
        , Instruction( 0x18d    , BURN_DOWN )
        , Instruction( 0x18e    , BURN_DOWN )
        , Instruction( 0x18f    , BURN_DOWN )
        , Instruction( 0x190    , BURN_DOWN )
        , Instruction( 0x191    , BURN_DOWN )
        , Instruction( 0x192    , BURN_DOWN )
        , Instruction( 0x193    , BURN_DOWN )
        , Instruction( 0x194    , BURN_DOWN )
        , Instruction( 0x195    , BURN_DOWN )
        , Instruction( 0x196    , BURN_DOWN )
        , Instruction( 0x197    , BURN_DOWN )
        , Instruction( 0x198    , BURN_DOWN )
        , Instruction( 0x199    , BURN_DOWN )
        , Instruction( 0x19a    , BURN_DOWN )
        , Instruction( 0x19b    , BURN_DOWN )
        , Instruction( 0x19c    , BURN_DOWN )
        , Instruction( 0x19d    , BURN_DOWN )
        , Instruction( 0x19e    , BURN_DOWN )
        , Instruction( 0x19f    , BURN_DOWN )
        , Instruction( 0x1a0    , BURN_DOWN )
        , Instruction( 0x1a1    , BURN_DOWN )
        , Instruction( 0x1a2    , BURN_DOWN )
        , Instruction( 0x1a3    , BURN_DOWN )
        , Instruction( 0x1a4    , BURN_DOWN )
        , Instruction( 0x1a5    , BURN_DOWN )
        , Instruction( 0x1a6    , BURN_DOWN )
        , Instruction( 0x1a7    , BURN_DOWN )
        , Instruction( 0x1a8    , BURN_DOWN )
        , Instruction( 0x1a9    , BURN_DOWN )
        , Instruction( 0x1aa    , BURN_DOWN )
        , Instruction( 0x1ab    , BURN_DOWN )
        , Instruction( 0x1ac    , BURN_DOWN )
        , Instruction( 0x1ad    , BURN_DOWN )
        , Instruction( 0x1ae    , BURN_DOWN )
        , Instruction( 0x1af    , BURN_DOWN )
        , Instruction( 0x1b0    , BURN_DOWN )
        , Instruction( 0x1b1    , BURN_DOWN )
        , Instruction( 0x1b2    , BURN_DOWN )
        , Instruction( 0x1b3    , BURN_DOWN )
        , Instruction( 0x1b4    , BURN_DOWN )
        , Instruction( 0x1b5    , BURN_DOWN )
        , Instruction( 0x1b6    , BURN_DOWN )
        , Instruction( 0x1b7    , BURN_DOWN )
        , Instruction( 0x1b8    , BURN_DOWN )
        , Instruction( 0x1b9    , BURN_DOWN )
        , Instruction( 0x1ba    , BURN_DOWN )
        , Instruction( 0x1bb    , BURN_DOWN )
        , Instruction( 0x1bc    , BURN_DOWN )
        , Instruction( 0x1bd    , BURN_DOWN )
        , Instruction( 0x1be    , BURN_DOWN )
        , Instruction( 0x1bf    , BURN_DOWN )
        , Instruction( 0x1c0    , BURN_DOWN )
        , Instruction( 0x1c1    , BURN_DOWN )
        , Instruction( 0x1c2    , BURN_DOWN )
        , Instruction( 0x1c3    , BURN_DOWN )
        , Instruction( 0x1c4    , BURN_DOWN )
        , Instruction( 0x1c5    , BURN_DOWN )
        , Instruction( 0x1c6    , BURN_DOWN )
        , Instruction( 0x1c7    , BURN_DOWN )
        , Instruction( 0x1c8    , BURN_DOWN )
        , Instruction( 0x1c9    , BURN_DOWN )
        , Instruction( 0x1ca    , BURN_DOWN )
        , Instruction( 0x1cb    , BURN_DOWN )
        , Instruction( 0x1cc    , BURN_DOWN )
        , Instruction( 0x1cd    , BURN_DOWN )
        , Instruction( 0x1ce    , BURN_DOWN )
        , Instruction( 0x1cf    , BURN_DOWN )
        , Instruction( 0x1d0    , BURN_DOWN )
        , Instruction( 0x1d1    , BURN_DOWN )
        , Instruction( 0x1d2    , BURN_DOWN )
        , Instruction( 0x1d3    , BURN_DOWN )
        , Instruction( 0x1d4    , BURN_DOWN )
        , Instruction( 0x1d5    , BURN_DOWN )
        , Instruction( 0x1d6    , BURN_DOWN )
        , Instruction( 0x1d7    , BURN_DOWN )
        , Instruction( 0x1d8    , BURN_DOWN )
        , Instruction( 0x1d9    , BURN_DOWN )
        , Instruction( 0x1da    , BURN_DOWN )
        , Instruction( 0x1db    , BURN_DOWN )
        , Instruction( 0x1dc    , BURN_DOWN )
        , Instruction( 0x1dd    , BURN_DOWN )
        , Instruction( 0x1de    , BURN_DOWN )
        , Instruction( 0x1df    , BURN_DOWN )
        , Instruction( 0x1e0    , BURN_DOWN )
        , Instruction( 0x1e1    , BURN_DOWN )
        , Instruction( 0x1e2    , BURN_DOWN )
        , Instruction( 0x1e3    , BURN_DOWN )
        , Instruction( 0x1e4    , BURN_DOWN )
        , Instruction( 0x1e5    , BURN_DOWN )
        , Instruction( 0x1e6    , BURN_DOWN )
        , Instruction( 0x1e7    , BURN_DOWN )
        , Instruction( 0x1e8    , BURN_DOWN )
        , Instruction( 0x1e9    , BURN_DOWN )
        , Instruction( 0x1ea    , BURN_DOWN )
        , Instruction( 0x1eb    , BURN_DOWN )
        , Instruction( 0x1ec    , BURN_DOWN )
        , Instruction( 0x1ed    , BURN_DOWN )
        , Instruction( 0x1ee    , BURN_DOWN )
        , Instruction( 0x1ef    , BURN_DOWN )
        , Instruction( 0x1f0    , BURN_DOWN )
        , Instruction( 0x1f1    , BURN_DOWN )
        , Instruction( 0x1f2    , BURN_DOWN )
        , Instruction( 0x1f3    , BURN_DOWN )
        , Instruction( 0x1f4    , BURN_DOWN )
        , Instruction( 0x1f5    , BURN_DOWN )
        , Instruction( 0x1f6    , BURN_DOWN )
        , Instruction( 0x1f7    , BURN_DOWN )
        , Instruction( 0x1f8    , BURN_DOWN )
        , Instruction( 0x1f9    , BURN_DOWN )
        , Instruction( 0x1fa    , BURN_DOWN )
        , Instruction( 0x1fb    , BURN_DOWN )
        , Instruction( 0x1fc    , BURN_DOWN )
        , Instruction( 0x1fd    , BURN_DOWN )
        , Instruction( 0x1fe    , BURN_DOWN )
        , Instruction( 0x1ff    , BURN_DOWN )
        , Instruction( 0x200    , BURN_DOWN )
        , Instruction( 0x201    , BURN_DOWN )
        , Instruction( 0x202    , BURN_DOWN )
        , Instruction( 0x203    , BURN_DOWN )
        , Instruction( 0x204    , BURN_DOWN )
        , Instruction( 0x205    , BURN_DOWN )
        , Instruction( 0x206    , BURN_DOWN )
        , Instruction( 0x207    , BURN_DOWN )
        , Instruction( 0x208    , BURN_DOWN )
        , Instruction( 0x209    , BURN_DOWN )
        , Instruction( 0x20a    , BURN_DOWN )
        , Instruction( 0x20b    , BURN_DOWN )
        , Instruction( 0x20c    , BURN_DOWN )
        , Instruction( 0x20d    , BURN_DOWN )
        , Instruction( 0x20e    , BURN_DOWN )
        , Instruction( 0x20f    , BURN_DOWN )
        , Instruction( 0x210    , BURN_DOWN )
        , Instruction( 0x211    , BURN_DOWN )
        , Instruction( 0x212    , BURN_DOWN )
        , Instruction( 0x213    , BURN_DOWN )
        , Instruction( 0x214    , BURN_DOWN )
        , Instruction( 0x215    , BURN_DOWN )
        , Instruction( 0x216    , BURN_DOWN )
        , Instruction( 0x217    , BURN_DOWN )
        , Instruction( 0x218    , BURN_DOWN )
        , Instruction( 0x219    , BURN_DOWN )
        , Instruction( 0x21a    , BURN_DOWN )
        , Instruction( 0x21b    , BURN_DOWN )
        , Instruction( 0x21c    , BURN_DOWN )
        , Instruction( 0x21d    , BURN_DOWN )
        , Instruction( 0x21e    , BURN_DOWN )
        , Instruction( 0x21f    , BURN_DOWN )
        , Instruction( 0x220    , BURN_DOWN )
        , Instruction( 0x221    , BURN_DOWN )
        , Instruction( 0x222    , BURN_DOWN )
        , Instruction( 0x223    , BURN_DOWN )
        , Instruction( 0x224    , BURN_DOWN )
        , Instruction( 0x225    , BURN_DOWN )
        , Instruction( 0x226    , BURN_DOWN )
        , Instruction( 0x227    , BURN_DOWN )
        , Instruction( 0x228    , BURN_DOWN )
        , Instruction( 0x229    , BURN_DOWN )
        , Instruction( 0x22a    , BURN_DOWN )
        , Instruction( 0x22b    , BURN_DOWN )
        , Instruction( 0x22c    , BURN_DOWN )
        , Instruction( 0x22d    , BURN_DOWN )
        , Instruction( 0x22e    , BURN_DOWN )
        , Instruction( 0x22f    , BURN_DOWN )
        , Instruction( 0x230    , BURN_DOWN )
        , Instruction( 0x231    , BURN_DOWN )
        , Instruction( 0x232    , BURN_DOWN )
        , Instruction( 0x233    , BURN_DOWN )
        , Instruction( 0x234    , BURN_DOWN )
        , Instruction( 0x235    , BURN_DOWN )
        , Instruction( 0x236    , BURN_DOWN )
        , Instruction( 0x237    , BURN_DOWN )
        , Instruction( 0x238    , BURN_DOWN )
        , Instruction( 0x239    , BURN_DOWN )
        , Instruction( 0x23a    , BURN_DOWN )
        , Instruction( 0x23b    , BURN_DOWN )
        , Instruction( 0x23c    , BURN_DOWN )
        , Instruction( 0x23d    , BURN_DOWN )
        , Instruction( 0x23e    , BURN_DOWN )
        , Instruction( 0x23f    , BURN_DOWN )
        , Instruction( 0x240    , BURN_DOWN )
        , Instruction( 0x241    , BURN_DOWN )
        , Instruction( 0x242    , BURN_DOWN )
        , Instruction( 0x243    , BURN_DOWN )
        , Instruction( 0x244    , BURN_DOWN )
        , Instruction( 0x245    , BURN_DOWN )
        , Instruction( 0x246    , BURN_DOWN )
        , Instruction( 0x247    , BURN_DOWN )
        , Instruction( 0x248    , BURN_DOWN )
        , Instruction( 0x249    , BURN_DOWN )
        , Instruction( 0x24a    , BURN_DOWN )
        , Instruction( 0x24b    , BURN_DOWN )
        , Instruction( 0x24c    , BURN_DOWN )
        , Instruction( 0x24d    , BURN_DOWN )
        , Instruction( 0x24e    , BURN_DOWN )
        , Instruction( 0x24f    , BURN_DOWN )
        , Instruction( 0x250    , BURN_DOWN )
        , Instruction( 0x251    , BURN_DOWN )
        , Instruction( 0x252    , BURN_DOWN )
        , Instruction( 0x253    , BURN_DOWN )
        , Instruction( 0x254    , BURN_DOWN )
        , Instruction( 0x255    , BURN_DOWN )
        , Instruction( 0x256    , BURN_DOWN )
        , Instruction( 0x257    , BURN_DOWN )
        , Instruction( 0x258    , BURN_DOWN )
        , Instruction( 0x259    , BURN_DOWN )
        , Instruction( 0x25a    , BURN_DOWN )
        , Instruction( 0x25b    , BURN_DOWN )
        , Instruction( 0x25c    , BURN_DOWN )
        , Instruction( 0x25d    , BURN_DOWN )
        , Instruction( 0x25e    , BURN_DOWN )
        , Instruction( 0x25f    , BURN_DOWN )
        , Instruction( 0x260    , BURN_DOWN )
        , Instruction( 0x261    , BURN_DOWN )
        , Instruction( 0x262    , BURN_DOWN )
        , Instruction( 0x263    , BURN_DOWN )
        , Instruction( 0x264    , BURN_DOWN )
        , Instruction( 0x265    , BURN_DOWN )
        , Instruction( 0x266    , BURN_DOWN )
        , Instruction( 0x267    , BURN_DOWN )
        , Instruction( 0x268    , BURN_DOWN )
        , Instruction( 0x269    , BURN_DOWN )
        , Instruction( 0x26a    , BURN_DOWN )
        , Instruction( 0x26b    , BURN_DOWN )
        , Instruction( 0x26c    , BURN_DOWN )
        , Instruction( 0x26d    , BURN_DOWN )
        , Instruction( 0x26e    , BURN_DOWN )
        , Instruction( 0x26f    , BURN_DOWN )
        , Instruction( 0x270    , BURN_DOWN )
        , Instruction( 0x271    , BURN_DOWN )
        , Instruction( 0x272    , BURN_DOWN )
        , Instruction( 0x273    , BURN_DOWN )
        , Instruction( 0x274    , BURN_DOWN )
        , Instruction( 0x275    , BURN_DOWN )
        , Instruction( 0x276    , BURN_DOWN )
        , Instruction( 0x277    , BURN_DOWN )
        , Instruction( 0x278    , BURN_DOWN )
        , Instruction( 0x279    , BURN_DOWN )
        , Instruction( 0x27a    , BURN_DOWN )
        , Instruction( 0x27b    , BURN_DOWN )
        , Instruction( 0x27c    , BURN_DOWN )
        , Instruction( 0x27d    , BURN_DOWN )
        , Instruction( 0x27e    , BURN_DOWN )
        , Instruction( 0x27f    , BURN_DOWN )
        , Instruction( 0x280    , BURN_DOWN )
        , Instruction( 0x281    , BURN_DOWN )
        , Instruction( 0x282    , BURN_DOWN )
        , Instruction( 0x283    , BURN_DOWN )
        , Instruction( 0x284    , BURN_DOWN )
        , Instruction( 0x285    , BURN_DOWN )
        , Instruction( 0x286    , BURN_DOWN )
        , Instruction( 0x287    , BURN_DOWN )
        , Instruction( 0x288    , BURN_DOWN )
        , Instruction( 0x289    , BURN_DOWN )
        , Instruction( 0x28a    , BURN_DOWN )
        , Instruction( 0x28b    , BURN_DOWN )
        , Instruction( 0x28c    , BURN_DOWN )
        , Instruction( 0x28d    , BURN_DOWN )
        , Instruction( 0x28e    , BURN_DOWN )
        , Instruction( 0x28f    , BURN_DOWN )
        , Instruction( 0x290    , BURN_DOWN )
        , Instruction( 0x291    , BURN_DOWN )
        , Instruction( 0x292    , BURN_DOWN )
        , Instruction( 0x293    , BURN_DOWN )
        , Instruction( 0x294    , BURN_DOWN )
        , Instruction( 0x295    , BURN_DOWN )
        , Instruction( 0x296    , BURN_DOWN )
        , Instruction( 0x297    , BURN_DOWN )
        , Instruction( 0x298    , BURN_DOWN )
        , Instruction( 0x299    , BURN_DOWN )
        , Instruction( 0x29a    , BURN_DOWN )
        , Instruction( 0x29b    , BURN_DOWN )
        , Instruction( 0x29c    , BURN_DOWN )
        , Instruction( 0x29d    , BURN_DOWN )
        , Instruction( 0x29e    , BURN_DOWN )
        , Instruction( 0x29f    , BURN_DOWN )
        , Instruction( 0x2a0    , BURN_DOWN )
        , Instruction( 0x2a1    , BURN_DOWN )
        , Instruction( 0x2a2    , BURN_DOWN )
        , Instruction( 0x2a3    , BURN_DOWN )
        , Instruction( 0x2a4    , BURN_DOWN )
        , Instruction( 0x2a5    , BURN_DOWN )
        , Instruction( 0x2a6    , BURN_DOWN )
        , Instruction( 0x2a7    , BURN_DOWN )

        , Instruction( 0x300   , END_TEST )
        , Instruction( 0x400   , END_TEST )
    };
    runTestSequence( sequence, verbose );
}

int main( void ) {
    //test_FULL( 1 );
    //test_LONG( 1 );
    //test_rootAUTO( 1 );
    //test_AUTO( 1 );
    test_burnDown( 1 );

  return 0;
}
