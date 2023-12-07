uint8_t Tree::getTreeParent( uint8_t child ) {
  return parentList[ child ];
}

void Graph::makeSpanningTree( uint8_t root ) {
// Construct a spanning tree of the graph, and return a list of each node's parent in the tree.
  spanningTree.parentList[ root ] = NONE;
  uint8_t treeSize = 1;

  Queue queue;
  queue.enqueue( root );
  while ( not queue.isEmpty() ) {
    uint8_t parent = queue.dequeue();
    ShuffledList adjacent;
    adjacent.setSize( numberAdjacent[ parent ] );
    adjacent.setList( adjacencyList[ parent ] );
    while ( uint8_t vertex = adjacent.next() ) {
      if ( spanningTree.parentList[ vertex ] ) { 
        continue; 
      }
      spanningTree.parentList[ vertex ] = parent;
      if ( ++treeSize == 16 ) {
        break;
      }
      queue.enqueue( vertex );
    }
  }
}


// ============================================================================================================================================
//          SHUFFLED LIST
// ============================================================================================================================================

uint8_t ShuffledList::next() {
// Return the next element from the list.
  if ( size == 0 ) {
    return NONE;
  }
  return list[ --size ];
}


void ShuffledList::setList( uint8_t array[], uint8_t arraySize ) {
  list = array;
  size = arraySize;
}


void ShuffledList::shuffle() {
// Shuffle the list in place using Fisher-Yates algorithm.
  uint8_t i = size;
  uint8_t j;
  Lantern* temp;
  while ( i > 1 ) {
    j = Random::urandom( i-- );
    temp = list[ i ];
    list[ i ] = list[ j ];
    list[ j ] = temp;
  }
}


#if MONITOR_MAKE_TREE
void ShuffledList::print() {
// Print the list to serial monitor.
  for ( uint8_t i=0; i < size; i++ ) {
    Serial.print( list[ i ] ); 
    Serial.print( "->" );
  }
  Serial.println();
}
#endif
