#include "TransitionMatrix.h"
#include "TransitionMatrix.cpp"

void test_TransitionMatrix( void ) {
  const StateTransition T[3] = { 
    { 0x00, 0x01, 0x02, 0x04 },
    { 0x02, 0x00, 0x03, 0x40 },
    { 0x02, 0x04, 0x06, 0x86 } 
  };
  const TransitionMatrix transitionMatrix = TransitionMatrix( 3, T );
}


void test_getNextState( void ) {
  std::cout << "Test getNextState." << std::endl;
  int errorCount = 0;
  const StateTransition T[3] = { 
     { 0x00, 0x01, 0x02, 0x04 },
     { 0x02, 0x00, 0x03, 0x40 },
     { 0x02, 0x04, 0x06, 0x86 } 
  };
  const TransitionMatrix transitionMatrix = TransitionMatrix( 3, T );

  if ( transitionMatrix.getNextState( 0 ) != 0x02 ) {
    errorCount++;
  }
  if ( transitionMatrix.getNextState( 1 ) != 0x03 ) {
    errorCount++;
  }
  if ( transitionMatrix.getNextState( 2 ) != 0x06 ) {
    errorCount++;
  }
  if ( transitionMatrix.getNextState( 3 ) != 0xFF ) {
    errorCount++;
  }
  if ( errorCount ) {
    std::cout << errorCount << " errors." << std::endl;
  }
  else {
    std::cout << "OK." << std::endl;
  }
}


void test_getNextOutput( void ) {
  std::cout << "Test getNextOutput." << std::endl;
  int errorCount = 0;
  const StateTransition T[3] = { 
     { 0x00, 0x01, 0x02, 0x04 },
     { 0x02, 0x00, 0x03, 0x40 },
     { 0x02, 0x04, 0x06, 0x86 } 
  };
  const TransitionMatrix transitionMatrix = TransitionMatrix( 3, T );

  if ( transitionMatrix.getNextOutput( 0 ) != 0x04 ) {
    errorCount++;
  }
  if ( transitionMatrix.getNextOutput( 1 ) != 0x40 ) {
    errorCount++;
  }
  if ( transitionMatrix.getNextOutput( 2 ) != 0x86 ) {
    errorCount++;
  }
  if ( transitionMatrix.getNextOutput( 3 ) != 0xFF ) {
    errorCount++;
  }
  if ( errorCount ) {
    std::cout << errorCount << " errors." << std::endl;
  }
  else {
    std::cout << "OK." << std::endl;
  }
}


void test_find( void ) {
  std::cout << "Test find." << std::endl;
  int errorCount = 0;
  const StateTransition T[3] = { 
     { 0x00, 0x01, 0x02, 0x04 },
     { 0x02, 0x00, 0x03, 0x40 },
     { 0x02, 0x04, 0x06, 0x86 } 
  };
  const TransitionMatrix transitionMatrix = TransitionMatrix( 3, T );

  uint8_t index = transitionMatrix.find( 0x00, 0x01 );
  if ( index != 0 ) {
    errorCount++;
    std::cout << "returned index " << ( int ) index << ". Should be 0." << std::endl;
  }
  index = transitionMatrix.find( 0x02, 0x00 );
  if ( index != 1 ) {
    errorCount++;
    std::cout << "returned index " << ( int ) index << ". Should be 1." << std::endl;
  }
  index = transitionMatrix.find( 0x02, 0x04 );
  if ( index != 2 ) {
    errorCount++;
    std::cout << "returned index " << ( int ) index << ". Should be 2." << std::endl;
  }
  index = transitionMatrix.find( 0x05, 0x04 );
  if ( index != 255 ) {
    errorCount++;
    std::cout << "{ 0x05, 0x04 } found at index " << ( int ) index << ". Should be 255." << std::endl;
  }
  index = transitionMatrix.find( 0x00, 0x00 );
  if ( index != 255 ) {
    errorCount++;
    std::cout << "{ 0x00, 0x00 } found at index " << ( int ) index << ". Should be 255." << std::endl;
  }
  if ( errorCount ) {
    std::cout << errorCount << " errors." << std::endl;
  }
  else {
    std::cout << "OK." << std::endl;
  }
}




int main( void ) {
  test_TransitionMatrix();
  test_find();
  test_getNextState();
  test_getNextOutput();
  
  return 0;
}
