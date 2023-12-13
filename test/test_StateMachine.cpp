#include "StateMachine.h"
#include "StateMachine.cpp"

void test_StateMachine( void ) {
  const StateTransition T[3] = { 
    { 0x00, 0x01, 0x02, 0x04 },
    { 0x02, 0x00, 0x03, 0x40 },
    { 0x02, 0x04, 0x06, 0x86 } 
  };
  const TransitionMatrix transitionMatrix = TransitionMatrix( 3, T );
  StateMachine S = StateMachine( transitionMatrix );
}


void test_next( void ) {
  const StateTransition T[3] = { 
    { 0x00, 0x01, 0x02, 0x04 },
    { 0x02, 0x00, 0x03, 0x40 },
    { 0x02, 0x04, 0x06, 0x86 } 
  };
  const TransitionMatrix transitionMatrix = TransitionMatrix( 3, T );
  StateMachine S = StateMachine( transitionMatrix );

  std::cout << "Test next" << std::endl;
  S.next();
  if ( S.getState() != 0 ) {
    std::cout << "failed. state = " << ( int ) S.getState() << " != 0" << std::endl;
  }
  else {
    std::cout << "OK." << std::endl;
  }

  S.setInput( 1 );
  S.next();
  if ( S.getState() != 2 ) {
    std::cout << "failed. state = " << ( int ) S.getState() << " != 2" << std::endl;
  }
  else {
    std::cout << "OK." << std::endl;
  }

  S.setInput( 5 );
  S.next();
  if ( S.getState() != 2 ) {
    std::cout << "failed. state = " << ( int ) S.getState() << " != 2" << std::endl;
  }
  else {
    std::cout << "OK." << std::endl;
  }

  S.setInput( 0 );
  S.next();
  if ( S.getOutput() != 0x40 ) {
    std::cout << "failed. output = " << ( int ) S.getOutput() << " != 0x40" << std::endl;
  }
  else {
    std::cout << "OK." << std::endl;
  }
}


void test_pushInput( void ) {
  const StateTransition T[3] = { 
    { 0x00, 0x01, 0x02, 0x04 },
    { 0x02, 0x00, 0x03, 0x40 },
    { 0x02, 0x04, 0x06, 0x86 } 
  };
  const TransitionMatrix transitionMatrix = TransitionMatrix( 3, T );
  StateMachine S = StateMachine( transitionMatrix );

  std::cout << "Test pushInput" << std::endl;
  S.pushInput(1);
  std::cout << "push(1): ";
  if ( S.getInput() != 1 ) {
    std::cout << "failed. input = " << ( int ) S.getInput() << " != 1" << std::endl;
  }
  else {
    std::cout << "OK." << std::endl;
  }
  S.pushInput(0);
  std::cout << "push(0): ";
  if ( S.getInput() != 2 ) {
    std::cout << "failed. input = " << ( int ) S.getInput() << " != 2" << std::endl;
  }
  else {
    std::cout << "OK." << std::endl;
  }
  S.pushInput(1);
  std::cout << "push(1): ";
  if ( S.getInput() != 5 ) {
    std::cout << "failed. input = " << ( int ) S.getInput() << " != 5" << std::endl;
  }
  else {
    std::cout << "OK." << std::endl;
  }
}


int main( void ) {
  test_StateMachine();
  test_pushInput();
  test_next();
  
  return 0;
}
