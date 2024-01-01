/*
============================================================================================================================================
             SHUFFLED LIST
============================================================================================================================================
*/

#include "ShuffledList.h"


ShuffledList::ShuffledList( uint16_t list, uint8_t size ) 
    : list( list )
    , size( size )
{
    shuffle();
}


uint8_t ShuffledList::next() 
// Return the next element from the list.
{
    if ( size == 0 ) {
        return NONE;
    }
    return getElement( --size );
}


void ShuffledList::shuffle() 
// Shuffle the list in place using Fisher-Yates algorithm.
{
    uint8_t i = size;
    uint8_t j;
    uint8_t temp;
    while ( i > 1 ) {
        j = Random::urandom( i-- );
        temp = getElement( i );
        setElement( i, getElement( j ) );
        setElement( j, temp );
    }
}


bool ShuffledList::isEmpty() 
{
    return size == 0;
}


uint8_t ShuffledList::getElement( uint8_t index ) 
{
    return ( list >> 4 * index ) & 0xF;
}


void ShuffledList::setElement( uint8_t index, uint8_t value ) 
{
    list &= ~( 0xF << 4 * index );
    list |= ( 0xF & value ) << 4 * index;
}
