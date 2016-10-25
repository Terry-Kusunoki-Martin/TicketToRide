#ifndef EDGE_H_
#define EDGE_H_

/**
 * An Edge is a class that represents a directed, weighted edge in a graph.
 * It is templated on three types:
 *   V:  a type to represent the vertex labels, and 
 *   E:  a type to represent the edge labels, and 
 *   W:  a type to represent the weight (usually an int, float, or double).
 */
template <typename V, typename E, typename W>
class Edge {
  protected:
    E label;   // The name of this edge.
    V* src;    // The source, or head, vertex for this edge.
    V* dest;   // The destination, or tail, vertex for this edge.
    W weight;  // The weight of this edge.

  public:
    Edge();
    Edge(V* src, V* dest, E label, W weight = 1);

    E getLabel();
    V getSrc();
    V getDest();
    W getWeight();

  friend class Graph<V,E,W>;
};

#include "edge-inl.h"

#endif
