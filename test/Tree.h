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
    Tree( uint8_t* parentList, uint8_t size );
    void setParent( uint8_t child, uint8_t parent );
    uint8_t getParent( uint8_t index );
    uint8_t getSize();

  private:
    uint8_t* parentList;
    uint8_t size;
};
#endif
