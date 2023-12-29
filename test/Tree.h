/*
======================================================================================================================================================
            TREE
======================================================================================================================================================

Minimal tree implimentation.
Does not allocate memory on the heap.
Tree structure is defined as a list of each nodes parents.
*/

#ifndef TREE_H
#define TREE_H

#include <cstdint>

#define NONE    0xFF

class Tree {
  public:
    Tree( const uint8_t size, uint8_t* parentList );
    uint8_t getSize();
    uint8_t getParent( uint8_t idx );
    void setParent( uint8_t idx, uint8_t parent );

  private:
    const uint8_t size;
    uint8_t* parentList;
};
#endif
