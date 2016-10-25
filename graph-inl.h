#include <stdexcept>
#include <fstream>

/**
 * Constructs an empty graph, containing no edges or vertices.
 */
template <typename V, typename E, typename W>
Graph<V,E,W>::Graph() {/*do nothing*/ }


/**
 * Destructs a graph.
 */
template <typename V, typename E, typename W>
Graph<V,E,W>::~Graph() {
  Queue<Pair<V,V*> >* vertex_it = vertexCache.getItems();
  while (!vertex_it->isEmpty()) {
    delete vertex_it->dequeue().second;
  }
  delete vertex_it;

  Queue<Pair<V,vector<Edge<V,E,W> >* > >* edge_it = outgoingEdges.getItems();
  while (!edge_it->isEmpty()) {
    delete edge_it->dequeue().second;
  }
  delete edge_it;
}


/**
 * Inserts a vertex into the graph
 * @param v - vertex label
 * @error throws runtime_error if the given vertex
 * was already in the graph.
 */
template <typename V, typename E, typename W>
void Graph<V,E,W>::insertVertex(V v) {
  if (vertexCache.containsKey(v)) {
    throw std::runtime_error("Tried to Graph::insertVertex duplicate vertex.");
  }
  V* p = new V(v);
  vertexCache.insert(v,p);
  outgoingEdges.insert(v, new vector<Edge<V,E,W> >());
}


/**
 * Removes a vertex and all its associated edges (outgoing and incoming)
 * from the graph.  
 * @param v - Vertex label for vertex to remove
 * @error throws runtime_error if the given vertex is not in the graph.
 */
template <typename V, typename E, typename W>
void Graph<V,E,W>::removeVertex(V v) {
  if(!containsVertex(v)){
    throw std::runtime_error("Attempted to Graph::removeVertex on vertex not in graph");
  }
  V* p = vertexCache.get(v);

  delete outgoingEdges.get(v); // deletes edge list
  outgoingEdges.remove(v);     // deletes vertex

  //Remove edges where v is the destination
  Queue<Pair<V,vector<Edge<V,E,W> >* > >* it = outgoingEdges.getItems();
  while (!it->isEmpty()) {
    vector<Edge<V,E,W> >* edgeList = it->dequeue().second;
    for (int i = edgeList->size()-1; i >= 0; --i) {
      if ((*edgeList)[i].dest == p) {
        edgeList->erase(edgeList->begin()+i);
      }
    }
  }
  delete it;
  delete p;
  vertexCache.remove(v);
}


/**
 * inserts a new edge into the graph.
 * @param src - source vertex for the directed edge.  If src is not already
 *              in the graph, it is inserted as a vertex
 * @param dest - destination vertex for the edge.  If dest is not already in
 *              the graph, it is inserted as a vertex
 * @param label - label for edge
 * @param weight (optional) - weight for edge; 1 if not provided
 * @error throws runtime_error if edge already exists
 */
template <typename V, typename E, typename W>
void Graph<V,E,W>::insertEdge(V src, V dest, E label, W weight) {
  if (!containsVertex(src)) {
    insertVertex(src);
  }
  if (!containsVertex(dest)) {
    insertVertex(dest);
  }
  if(containsEdge(src, dest)){
    throw std::runtime_error("Attempted to Graph::insertEdge duplicate edge");
  }

  V* srcP = vertexCache.get(src);
  V* destP = vertexCache.get(dest);

  vector<Edge<V,E,W> >* edgeList = outgoingEdges.get(src);
  Edge<V,E,W> newEdge(srcP, destP, label, weight);

  edgeList->push_back(newEdge);
}


/**
 * Removes an edge from src->dest from the graph
 * @param src - vertex label of source node
 * @param dest - vertex label of destination node
 * @error throws runtime_error if src->dest is not in the graph.
 */
template <typename V, typename E, typename W>
void Graph<V,E,W>::removeEdge(V src, V dest) {
  if (!containsEdge(src, dest)) {
    throw std::runtime_error("attempted to Graph::removeEdge called on edge not in graph");
  }

  V* srcP = vertexCache.get(src);
  V* destP = vertexCache.get(dest);
  vector<Edge<V,E,W> >* edgeList = outgoingEdges.get(src);
  for (int i = 0; i < edgeList->size(); ++i) {
    if ((*edgeList)[i].src == srcP && (*edgeList)[i].dest == destP) {
      edgeList->erase(edgeList->begin()+i);
      return;
    }
  }
}


/**
 * Returns true if the graph contains the given vertex.
 * @param v - label of vertex to search for
 * @return bool - true if vertex exists
 */
template <typename V, typename E, typename W>
bool Graph<V,E,W>::containsVertex(V v) {
  return vertexCache.containsKey(v);
}

/**
 * Returns true if the graph contains the given src->dest edge.
 * @param src - source vertex
 * @param dest - destination vertex
 * @return bool - true if edge exists
 */
template <typename V, typename E, typename W>
bool Graph<V,E,W>::containsEdge(V src, V dest) {
  if (!containsVertex(src) || !containsVertex(dest)) {
    return false;
  }
  V* destP = vertexCache.get(dest);
  vector<Edge<V,E,W> >* edgeList = outgoingEdges.get(src);
  for (int i = 0; i < edgeList->size(); ++i) {
    if ((*edgeList)[i].dest == destP) {
      return true;
    }
  }
  return false;
}

/**
 * Returns a vector of all edges in the graph.
 * @return vector< Edge<V,E,W> > - vector of Edge objects, one for
 *  each edge in the graph
 */
template <typename V, typename E, typename W>
vector<Edge<V,E,W> > Graph<V,E,W>::getEdges() {
  vector<Edge<V,E,W> > rv;
  Queue<Pair<V,vector<Edge<V,E,W> >* > >* it = outgoingEdges.getItems();
  while (!it->isEmpty()) {
    vector<Edge<V,E,W> >* edgeList = it->dequeue().second;
    for (int i = 0; i < edgeList->size(); ++i) {
      rv.push_back((*edgeList)[i]);
    }
  }
  delete it;
  return rv;
}


/**
 * Returns a vector of all vertices in the graph.
 * @return vector<V> - vector of vertex labels
 */
template <typename V, typename E, typename W>
vector<V> Graph<V,E,W>::getVertices() {
  vector<V> rv;
  Queue<Pair<V,V* > >* it = vertexCache.getItems();
  while (!it->isEmpty()) {
    rv.push_back(it->dequeue().first);
  }
  delete it;
  return rv;
}


/**
 * Returns all outgoing edges from a source vertex
 * @param src - source vertex label
 * @return vector<Edge<V,E,W> > - vector of Edge objects, one for each 
 *    outgoing edge.
 */
template <typename V, typename E, typename W>
vector<Edge<V,E,W> > Graph<V,E,W>::getOutgoingEdges(V src) {
  return *(outgoingEdges.get(src));
}

/**
 * Returns all vertices adjacent to a source node.  
 * @param src - source vertex label for outgoing edges
 * @return vector<V> - a vector of all destination nodes connected to src by a 
 *   directed edge
 */
template <typename V, typename E, typename W>
vector<V> Graph<V,E,W>::getNeighbors(V src) {
  vector<V> neighbors;
  vector<Edge<V,E,W> >* outEdges = outgoingEdges.get(src);
  for (int i = 0; i < outEdges->size(); ++i) {
    neighbors.push_back((*outEdges)[i].getDest());
  }
  return neighbors;
}

/**
 * Returns all incoming edges to a destination vertex
 * @param dest - destination vertex label
 * @return vector<Edge<V,E,W> > - vector of Edge objects, one for each 
 *    incoming edge.
 */
template <typename V, typename E, typename W>
vector<Edge<V,E,W> > Graph<V,E,W>::getIncomingEdges(V dest) {
  vector<Edge<V,E,W> > rv;
  Queue<Pair<V,vector<Edge<V,E,W> >* > >* it = outgoingEdges.getItems();
  while (!it->isEmpty()) {
    vector<Edge<V,E,W> >* edgeList = it->dequeue().second;
    for (int i = 0; i < edgeList->size(); ++i) {
      if ((*edgeList)[i].getDest() == dest) {
        rv.push_back((*edgeList)[i]);
      }
    }
  }
  delete it;
  return rv;
}


/**
 * Returns the Edge between the given source and destination vertex
 * @param src - source vertex label
 * @param dest - destination vertex label
 * @return - Edge object between src and dest
 * @error - throws runtime_error if src->dest is not in the graph.
 */
template <typename V, typename E, typename W>
Edge<V,E,W> Graph<V,E,W>::getEdge(V src, V dest) {
  if(!containsEdge(src,dest)){
    throw std::runtime_error("Graph::getEdge called on edge not in graph");
  }    
  vector<Edge<V,E,W> >* edgeList = outgoingEdges.get(src);
  for (int i = 0; i < edgeList->size(); ++i) {
    if ((*edgeList)[i].getDest() == dest) {
      return (*edgeList)[i];
    }
  }  
}
    
/* outputs Graph object to file in GraphViz format.  We have not defined it
 * for directed graphs, but could if needed
 */
template <typename V, typename E, typename W>
void Graph<V,E,W>::graphViz(std::string filename) {
  throw std::runtime_error("This method is not defined for directed graphs");
}


/* << operator defined for the Graph class
 * @param out - output stream object to place output
 * @param g - Graph object to output
 * @return output stream used to output the graph
 */
template <typename V, typename E, typename W>
ostream& operator<<(ostream& out, Graph<V,E,W>& g) {
  Queue<Pair<V,vector<Edge<V,E,W> >* > >* it =  g.outgoingEdges.getItems();
  while (!it->isEmpty()) {
    Pair<V,vector<Edge<V,E,W> >* > next = it->dequeue();
    out << next.first << ": {";
    if (next.second->size() > 0) {
      out << (*(next.second))[0].getDest();
    }
    for (int i = 1; i < next.second->size(); ++i) {
      out << ", " << (*(next.second))[i].getDest();
    }
    out << "}\n";
  }
  delete it;
  return out;
}

