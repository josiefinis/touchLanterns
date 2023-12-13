// ======================================================================================================================================================
//          TREE
// ======================================================================================================================================================

#include "Tree.h"


Tree::Tree( uint8_t* parentList, uint8_t size ) 
  : parentList( parentList )
  , size( size )
{
  for ( uint8_t i=0; i<size; i++ ) {
    parentList[i] = NONE;
  }
}


void Tree::setParent( uint8_t child, uint8_t parent ) {
  parentList[ child ] = parent;
}


uint8_t Tree::getParent( uint8_t index ) {
  return parentList[ index ];
}


uint8_t Tree::getSize() {
  return size;
}
