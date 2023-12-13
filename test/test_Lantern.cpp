#include "Lantern.h"
#include "Lantern.cpp"

void test_Lantern( void ) {
  Lantern lantern = Lantern();
}


void updateCycle( Lantern& lantern, bool input ) {
  std::cout << ( int ) input;
  lantern.pushInput( input );
  lantern.nextState();
  lantern.updateOutput();
  for ( int j=0; j<16; j++ ) {
    lantern.changeBrightness();
    std::cout << "\t" << ( int ) lantern.getBrightness();
  }
  std::cout << std::endl;
}


void inputSequence( Lantern& lantern, uint32_t sequence ) {
  for ( int i=0; i<32; i++ ) {
    bool input = ( sequence >> i ) & 1;
    updateCycle( lantern, input );
  }
}


void test_fullUpFullDown() {
  Lantern lantern = Lantern();
  inputSequence( lantern, 0x44444444 );
}


void test_auto() {
  Lantern lantern = Lantern();
  inputSequence( lantern, 0x0010100F );
}


int main( void ) {
  test_Lantern();
  test_fullUpFullDown();
  test_auto();
  
  return 0;
}
