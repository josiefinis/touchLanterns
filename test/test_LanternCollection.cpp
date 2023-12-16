#include "LanternCollection.h"
#include "LanternCollection.cpp"


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


const uint16_t neighbourList[16] = 
{ 
  0x421 , 0x4320, 0x5310, 0x521 ,
  0x7610, 0x7632, 0xC854, 0xC854,
  0xA976, 0xDBA8, 0xB98 , 0xFA9 ,
  0xED76, 0xFEC9, 0xFDC , 0xEDB 
};

LanternCollection lanternCollection = LanternCollection( 16, neighbourList );


Result updateLantern( int t, Lantern& lantern, bool input, bool verbose=0 ) {
  Result result;
  lantern.pushSensor( input );
  result.input = lantern.prioritiseInput();
  if ( verbose ) {
    std::cout << std::endl << t << "\t" << ( int ) input << " T: "; 
    printState( lantern.getState() ); 
    std::cout << ",  "; 
    printInput( lantern.prioritiseInput() );
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

int main( void ) {

  return 0;
}

