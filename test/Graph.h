/*
======================================================================================================================================================
             GRAPH
======================================================================================================================================================

Minimal graph implementation that tries to be frugal with memory and does not allocate memory on the heap.

    Graph structure is defined by an adjacency list.
    It's only job is to create a random spanning tree with a specified vertex as root.
*/


#ifndef GRAPH_H
#define GRAPH_H

#include <cstdint>
#include "Tree.h"
#include "Queue.h"
#include "ShuffledList.h"


class Graph {
  public:
    Graph( uint8_t const vertexCount, const uint16_t* adjacencyList );
    uint8_t countAdjacent( uint8_t vertex );
    void makeSpanningTree( Tree* spanningTree, uint8_t root );

  private:
    const uint8_t vertexCount;
    const uint16_t* adjacencyList;
};
#endif
