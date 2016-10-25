#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>   
#include <iostream>
#include "library/hashTable.h"

using std::vector; 
using std::ostream;

//Forward declare Graph
template <typename V, typename E, typename W> class Graph;

#include "edge.h"
//Declare << operator so we can make it a friend
template <typename V, typename E, typename W>
ostream& operator<<(ostream& out, Graph<V,E,W>& g);

/**
 * A Graph is a class that represents a directed, weighted graph.
 * It is templated on three types:
 *   V:  a type to represent the vertex labels, and 
 *   E:  a type to represent the edge labels, and 
 *   W:  a type to represent the weight (usually an int, float, or double).
 *
 * This implementation represents the graph using edge adjacency lists
 */
template <typename V, typename E, typename W>
class Graph {
  protected:
    /* This graph is stored using an adjacency-list representation:
     * for each vertex v, we store the list of v's outgoing edges. */ 
    
    /* outgoingEdges is a dictionary of all the vertex-edge list pairs.*/
    HashTable< V, vector<Edge<V,E,W> >* > outgoingEdges;
 
    /* Caches vertex labels, so that we  can efficiently compare label
     * pointers instead of the labels themselves */
    HashTable<V,V*> vertexCache;

  public:
    
    /* Constructor: initially constructs a graph with no vertices or edges */
    Graph();
    virtual ~Graph();

    /* Returns all vertices in the grpah */
    vector<V>            getVertices(); 
    
    /* insert vertex v into the graph */
    void insertVertex(V v);
    
    /* remove vertex v from the graph.  Throws an exception if v is not in the
     *  graph */
    void removeVertex(V v);

    /* returns true if v is in the graph*/
    bool containsVertex(V v);           
                                         
    /* inserts a directed edge from src to dest with the given
     *    label and weight.  The weight defaults to 1 if not provided */
    virtual void insertEdge(V src, V dest, E label, W weight = 1);
    
    /* removes the edge from src to dest from the graph, throwing an exception
     * if the edge does not exist */
    virtual void removeEdge(V src, V dest);
        
    /* Returns true if the edge src->dest is in the graph */
    bool containsEdge(V src, V dest);   
    
    /* Returns the directed edge src->dest or throws an exception if the edge
     *  is not in the graph */
    Edge<V,E,W> getEdge(V src, V dest);  
    
    /* Returns all edges in the graph */
    virtual vector<Edge<V,E,W> > getEdges();   
              
    /* Returns all edges outgoing from a src node */
    vector<Edge<V,E,W> > getOutgoingEdges(V src);

    /* Returns all incomding edges for a destination node */
    vector<Edge<V,E,W> > getIncomingEdges(V dest); 

    /* Returns all the vertices connected by outgoing edge from src */
    vector<V>            getNeighbors(V src);

    /* Saves the graph in GraphViz format to the given file */
    virtual void graphViz(std::string filename);

    friend ostream& ::operator<< <V,E,W>(ostream& out, Graph<V,E,W>& g);
};


#include "graph-inl.h"

#endif
