#include <vector>
using std::vector;

#include "edge.h"
#include "graph.h"


/**
 * Returns whether two vertices dest is reachable from src in the Graph g using
 *  depth-first search
 * @param src - source vertex
 * @param dest - destination vertex
 * @param g - pointer to Graph object
 * @return true if a path exists
 */
template <typename V, typename E, typename W>
bool reachableDFS(V src, V dest, Graph<V,E,W>* g);


/**
 * Returns the shortest length path found from the vertex src
 * to the vertex dest, using an unweighted breadth-first search of the graph.
 * @param src - source vertex
 * @param dest - destination vertex
 * @param g - pointer to Graph object
 * @return vector<V> - sequence of vertices in path from src to dest
 * @error throws runtime_error if no path exists
 */
template <typename V, typename E, typename W>
vector<V> shortestLengthPathBFS(V src, V dest, Graph<V,E,W>* g);


/**
 * dijkstra calculates the single source minimum distance to all other
 *   vertices in the graph from a src node
 * @param src - source vertex
 * @param g - pointer to Graph object to search
 * @return Dictionary<V,W> - Dictionary of vertices and their distance
 *    from src
 */
template <typename V, typename E, typename W>
Dictionary<V,W>* dijkstras(V src, Graph<V,E,W>* g);


#include "graph-algorithms-inl.h"

