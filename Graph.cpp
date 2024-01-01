/*
======================================================================================================================================================
                 GRAPH
======================================================================================================================================================
*/
#ifndef GRAPH_CPP
#define GRAPH_CPP

#include "Graph.h"
#include "ShuffledList.h"
#include "Queue.h"


Graph::Graph( const uint8_t vertexCount, const uint16_t* adjacencyList )
    : vertexCount( vertexCount )
    , adjacencyList( adjacencyList )
{}


uint8_t Graph::countAdjacent( uint8_t vertex ) 
// Return the number of adjacent vertices to vertex.
{
    uint16_t adjacentVertices = adjacencyList[ vertex ];          // NOTE: It is intentional that e.g. adjacentVertices = 0x4320 will return 4 and 0x0432 will return 3.
    uint8_t count = 1;                                            //       Also that it assumes at least 1 adjacent vertex so that 0x0 will return 1.
    while ( adjacentVertices >>= 4 )                              //       This is to handle ambiguity and between 0 as 0-index and 0 as no vertex.
    {
        count++;
    }
    return count;
}


void Graph::makeSpanningTree( Tree* spanningTree, uint8_t root ) 
// Construct and return a random spanning tree of the graph, starting from root.
{
    uint8_t queueArray[10];
    Queue queue = Queue( queueArray, 10 );
    queue.enqueue( root );
    uint8_t treeSize = 1;
    while ( not queue.isEmpty() ) 
    {
        uint8_t parent = queue.dequeue();
        ShuffledList adjacentToParent = ShuffledList( adjacencyList[ parent ], countAdjacent( parent ) );

        while ( not adjacentToParent.isEmpty() ) 
        {
            uint8_t vertex = adjacentToParent.next(); 
            if ( spanningTree->getParent( vertex ) != NONE ) 
            { 
                continue; 
            }
            if ( vertex == root ) 
            { 
                continue; 
            }
            spanningTree->setParent( vertex, parent );
            if ( ++treeSize == vertexCount ) 
            {
              break;
            }
            queue.enqueue( vertex );
        }
    }
}
#endif
