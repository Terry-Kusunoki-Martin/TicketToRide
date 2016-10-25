#ifndef UGRAPH_H_
#define UGRAPH_H_
#include "graph.h"

/* class UGraph is an undirected graph representation.  It utilizes
 *   a directed graph as its underlying implementation, but redefines key
 *   operations to ensure the graph acts undirected
 */
template <typename V, typename E, typename W>
class UGraph: public Graph<V,E,W>{
  public:
    /* inserts a directed edge from src to dest with the given
     *    label and weight.  The weight defaults to 1 if not provided */
    void insertEdge(V src, V dest, E label, W weight = 1);
    
    /* removes the edge from src to dest from the graph, throwing an exception
     * if the edge does not exist */
    void removeEdge(V src, V dest);
    
    /* Returns all edges in the graph */
    vector<Edge<V,E,W> > getEdges();

    /* Saves the graph in GraphViz format to the given file */
    void graphViz(std::string filename);
};

#include "ugraph-inl.h"

#endif

