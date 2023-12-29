#include "StateTransition.h"
#include "StateTransition.cpp"

void test_StateTransition( void ) {
  const StateTransition x = { 0x00, 0x01, 0x02, 0x04 };
  std::cout << std::hex << "StateTransition : " << ( int ) x.state << ", " << ( int ) x.input << " --> " << ( int ) x.nextState << ", " << ( int ) x.nextOutput << std::dec << std::endl;
}


void test_lt( void ) {
  const StateTransition x = { 0x00, 0x01, 0x02, 0x04 };
  const StateTransition y = { 0x02, 0x00, 0x03, 0x40 };
  const StateTransition z = { 0x02, 0x04, 0x05, 0x85 };
  int errorCount = 0;
  
  std::cout << "Test lt: ( x < y )" << std::endl;
  std::cout << "x = ";
  x.print();
  std::cout << "y = ";
  y.print();
  if ( y < x ) { 
    std::cout << "  failed: y < x returned " << ( bool ) ( y < x ) << std::endl;
    errorCount++;
  }
  if ( not ( x < y ) ) { 
    std::cout << "  failed: !( x < y ) returned " << ( bool ) ( x < y ) << std::endl;
    errorCount++;
  }
  std::cout << "Test lt: ( y < z )" << std::endl;
  std::cout << "y = ";
  y.print();
  std::cout << "z = ";
  z.print();
  if ( z < y ) { 
    std::cout << "  failed: z < y returned true."  << std::endl;
    errorCount++;
  }
  if ( not ( y < z ) ) {
    std::cout << "  failed: !( y < z ) returned true."  << std::endl;
    errorCount++;
  }
  if ( x < x ) {
    std::cout << "  failed: x < x returned true."  << std::endl;
    errorCount++;
  }
  std::cout << "Test complete with " << errorCount << " errors." << std::endl;
}


void test_gt( void ) {
  const StateTransition x = { 0x00, 0x01, 0x02, 0x04 };
  const StateTransition y = { 0x02, 0x00, 0x03, 0x40 };
  const StateTransition z = { 0x02, 0x04, 0x05, 0x85 };
  int errorCount = 0;
  
  std::cout << "Test gt: ( y > x )" << std::endl;
  std::cout << "x = ";
  x.print();
  std::cout << "y = ";
  y.print();
  if ( x > y ) { 
    std::cout << "  failed: x > y returned " << ( bool ) ( x > y ) << std::endl;
    errorCount++;
  }
  if ( not ( y > x ) ) { 
    std::cout << "  failed: !( y > x ) returned " << ( bool ) ( y > x ) << std::endl;
    errorCount++;
  }
  std::cout << "Test lt: ( z > y )" << std::endl;
  std::cout << "y = ";
  y.print();
  std::cout << "z = ";
  z.print();
  if ( y > z ) { 
    std::cout << "  failed: y > z returned true."  << std::endl;
    errorCount++;
  }
  if ( not ( z > y ) ) {
    std::cout << "  failed: !( z > y ) returned true."  << std::endl;
    errorCount++;
  }
  if ( x > x ) {
    std::cout << "  failed: x > x returned true."  << std::endl;
    errorCount++;
  }
  std::cout << "Test complete with " << errorCount << " errors." << std::endl;
}


int main( void ) {
  test_StateTransition();
  test_lt();
  test_gt();

  return 0;
}
