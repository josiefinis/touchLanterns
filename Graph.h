#ifndef NEIGHBOUR_GRAPH_H
#define NEIGHBOUR_GRAPH_H


#define NONE                    0xFF
#define GRAPH_N_VERTICES        16
#define MAX_ADJACENT            4

struct Tree;

class Graph {
  public:
    void makeSpanningTree( uint8_t root );

  private:
    Tree spanningTree;
    uint8_t numberAdjacent[ NUMBER_VERTICES ];
    uint8_t adjacencyList[ NUMBER_VERTICES ][ MAX_ADJACENT ];
};


#define TREE_N_NODES            GRAPH_N_VERTICES

struct Tree {
  uint8_t getParent( uint8_t child );
  uint8_t parentList[ NUMBER_NODES ]
}


#define MAX_LIST_SIZE  4

class ShuffledList {
  public:
    uint8_t next( void );
    setList( uint8_t array[], uint8_t size );
    void shuffle( void );
    void print( void );

  private:
    uint8_t list[ MAX_LIST_SIZE ];
    uint8_t size;
};


#define MAX_QUEUE_SIZE  10

class Queue {
  public:
    Queue();
    bool isEmpty();
    bool enqueue( uint8_t value );
    uint8_t dequeue();
    void print();

  private:
    uint8_t queue[ MAX_QUEUE_SIZE ];             
    uint8_t first;
    uint8_t size;
};
#endif
