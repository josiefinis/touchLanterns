#include <cstdint>
#include <iostream>

class Thing {
  public:
    Thing()
      : var( 0 )
      , input( 0 )
      , output( 0 )
    {}

    void push( bool value ) {
      input <<= 1;
      input |= value;
    }
    
    uint8_t getVar( void ) {
      return var;
    }

    uint8_t getInput( void ) {
      return input;
    }

    uint8_t getOutput( void ) {
      return output;
    }

    void print( void ) {
      std::cout << ( int ) var << "\t" << ( int ) input << "\t" << ( int ) output << "\t" << std::endl;
    }

  private:
    uint8_t var;
    uint8_t input;
    uint8_t output;

};

int main( void ) {
  Thing thing;

  uint16_t x = 0x2069;
  for ( int i=1; i<50; i++ ) {
    x ^= x << 7;
    x ^= x >> 5;
    thing.push( x % 2 );
    std::cout << ( int ) thing.getVar() << "\t" << ( int ) thing.getInput() << "\t" << ( int ) thing.getOutput() << "\t" << std::endl;
    thing.print();
  }

  return 0;
}
