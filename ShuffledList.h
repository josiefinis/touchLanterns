/*
============================================================================================================================================
         SHUFFLED LIST
============================================================================================================================================

A list for returning a sequence of values in random order without repetition.
Tries to be frugal with memory and does not allocate memory on the heap.
Array implementation is more robust and more flexible.
uint16 implementation uses a list of at most four 4-bit values represented as a single 16 bit integer.
*/

#ifndef SHUFFLED_LIST_H
#define SHUFFLED_LIST_H

#define UINT16_IMPLEMENTATION       1
#define ARRAY_IMPLEMENTATION        !UINT16_IMPLEMENTATION

  #if UINT16_IMPLEMENTATION
  
  class ShuffledList {
    public:
      ShuffledList( uint16_t list, uint8_t size );
      uint8_t next( void );
      void shuffle( void );
      void print( void );
  
    private:
      uint16_t list;
      uint8_t size;
      uint8_t getElement( uint8_t index );
      void setElement( uint8_t index, uint8_t value );
  };
  #endif

  #if ARRAY_IMPLEMENTATION
  
  class ShuffledList {
    public:
      ShuffledList( uint8_t* list, uint8_t size );
      uint8_t next( void );
      void shuffle( void );
      void print( void );
  
    private:
      uint8_t* list;
      uint8_t size;
  };
  #endif

#endif
