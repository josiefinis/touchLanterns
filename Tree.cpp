/* 
======================================================================================================================================================
                                    TREE
======================================================================================================================================================
*/ 
#include "Tree.h"


Tree::Tree( const uint8_t size, uint8_t* parentList ) 
    : size( size )
    , parentList( parentList )
{
    for ( uint8_t i=0; i<size; i++ ) 
    {
        parentList[ i ] = NONE;
    }
}


uint8_t Tree::getSize() 
{
    return size;
}


uint8_t Tree::getParent( uint8_t idx ) 
{
    return parentList[ idx ];
}


void Tree::setParent( uint8_t idx, uint8_t parent ) 
{
    parentList[ idx ] = parent;
}
