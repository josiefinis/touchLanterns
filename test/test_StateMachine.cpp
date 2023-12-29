#include "StateMachine.h"
#include "StateMachine.cpp"

void test_StateMachine( void ) {
  const StateTransition T[4] = { 
    { 0x00, 0x01, 0x02, 0x04 },
    { 0x02, 0x00, 0x03, 0x40 },
    { 0x02, 0x04, 0x06, 0x86 },
    { 0x03, 0xFF, 0x01, 0x00 } 
  };
  const TransitionMatrix transitionMatrix = TransitionMatrix( 4, T );
  StateMachine S = StateMachine( transitionMatrix );
}


void test_next( void ) {
  const StateTransition T[4] = { 
    { 0x00, 0x01, 0x02, 0x04 },
    { 0x02, 0x00, 0x03, 0x40 },
    { 0x02, 0x04, 0x06, 0x86 },
    { 0x03, 0xFF, 0x01, 0x00 } 
  };
  const TransitionMatrix transitionMatrix = TransitionMatrix( 4, T );
  StateMachine S = StateMachine( transitionMatrix );

  std::cout << "Test next" << std::endl;
  S.next( 0 );
  if ( S.getState() != 0 ) {
    std::cout << "failed. state = " << ( int ) S.getState() << " != 0" << std::endl;
  }
  else {
    std::cout << "OK." << std::endl;
  }

  S.next( 1 );
  if ( S.getState() != 2 ) {
    std::cout << "failed. state = " << ( int ) S.getState() << " != 2" << std::endl;
  }
  else {
    std::cout << "OK." << std::endl;
  }

  S.next( 5 );
  if ( S.getState() != 2 ) {
    std::cout << "failed. state = " << ( int ) S.getState() << " != 2" << std::endl;
  }
  else {
    std::cout << "OK." << std::endl;
  }

  S.next( 0 );
  if ( S.getOutput() != 0x40 ) {
    std::cout << "failed. output = " << ( int ) S.getOutput() << " != 0x40" << std::endl;
  }
  else {
    std::cout << "OK." << std::endl;
  }

  S.next( 0 );
  if ( S.getState() != 0x01 ) {
    std::cout << "failed. output = " << ( int ) S.getState() << " != 0x01" << std::endl;
  }
  else {
    std::cout << "OK." << std::endl;
  }
}


int main( void ) {
  test_StateMachine();
  test_next();
  
  return 0;
}
