#include "library/stack.h"
#include "library/queue.h"
#include "library/priorityQueue.h"
#include "library/binaryHeap.h"
#include "library/arrayStack.h"
#include "library/arrayQueue.h"
#include "library/BST.h"
#include "library/linkedBST.h"
#include <iostream>
#include <limits>
using namespace std;
/**
 * Returns whether two vertices dest is reachable from src in the Graph g using
 *  depth-first search
 * @param src - source vertex
 * @param dest - destination vertex
 * @param g - pointer to Graph object
 * @return true if a path exists
 */
template <typename V, typename E, typename W>
bool reachableDFS(V src, V dest, Graph<V,E,W>* g){
  vector<V> neighbors;// = g->getNeighbors(src);  //get the neighbors (surrounding vertices) of the source
  BST<V,int>* vertexName = new LinkedBST<V,int>(); //tree that will store the name of neighbors to prevent repeats
  Stack<V>* DFS = new ArrayStack<V>;  //stack to use to search through neighbors
  DFS->push(src);  //insert into stack before beginning
  vertexName->insert(src,1); //insert into tree before begging 
  while(!DFS->isEmpty()){  //while the stack is not empty
    V temp = DFS->pop();  //pop the top off 
    if(temp == dest){    //if this vertex is the destination, exit fx and return true;
      delete DFS;
      delete vertexName;
      return true;
    }

    neighbors = g->getNeighbors(temp);  //get the neighbors of this temp vertex
    for(int i =0; i<neighbors.size(); i++){  //loop through the neighbors
      if(!vertexName->contains(neighbors[i])){ //if we haven't already visited temp's neighbor
       DFS->push(neighbors[i]);  //push this vertex onto the stack
       vertexName->insert(neighbors[i],1);  //add it to the tree since it has beeen visited 
      }
    }

   }
  delete vertexName;
  delete DFS;
  return false;  //return false if we never find a vertex that matches the destination
}
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
vector<V> shortestLengthPathBFS(V src, V dest, Graph<V,E,W>* g){
  vector<V> neighbors;  //vector for neighbors
  vector<V> path;  //vector to store the vertices to reach dest
  Queue<V>* BFS = new ArrayQueue<V>;  //queue used to search through the neighbors
  BST<V,V>* vertices = new LinkedBST<V,V>();  //tree used to store which vertices have been visited, and where we have visited from
  V temp; 
  bool found = false;  //helper bool: true when dest is reached.
  BFS->enqueue(src);  //begin by inserting into queue
  vertices->insert(src,src);  //mark the source as visited 
  while(!BFS->isEmpty()){  //while the queue isn't empty
    temp = BFS->dequeue();  //dequeue an item
    if(temp == dest){  //if this item is the dest
      found=true;  //set found = true and break out of the while loop
      break;
    }
    neighbors = g->getNeighbors(temp);  //get the neighbors of the dequeued item
    for(int i =0; i<neighbors.size(); i++){  //loop through item's neighbors
      if(!vertices->contains(neighbors[i])){  //if item's neighbor hasn't been visited
        BFS->enqueue(neighbors[i]);  //add it to the queue
        vertices->insert(neighbors[i],temp);  //mark it as visitied, and where we came from
      }
    }
  }
  
  if (!found){  //if the destination is never reached throw an error
    throw std::runtime_error("No path exists");
  }
  while(temp!=src){  //until we step back to the src
    path.insert(path.begin(),temp);  //insert into the path the vertices to the dest starting with the one preceding dest
    temp=vertices->find(temp);   //trace back the vertex just before the inserted one
  }
  path.insert(path.begin(),src);  //insert the source. 

  delete BFS;  
  delete vertices;
  return path;  //return path giving shortest way to dest
}


/**
 * dijkstra calculates the single source minimum distance to all other
 *   vertices in the graph from a src node
 * @param src - source vertex
 * @param g - pointer to Graph object to search
 * @return Dictionary<V,W> - Dictionary of vertices and their distance
 *    from src
 */
template <typename V, typename E, typename W>
Dictionary<V,W>* dijkstras(V src, Graph<V,E,W>* g){
  vector<V> vertices = g->getVertices(); //grab vertices of graph 
  Dictionary<V,W>* distances=new HashTable<V,W>;  //store distances from src to every vertex
  PriorityQueue<W,V>* PQ=new BinaryHeap<W,V>;  
  V v, u;
  vector<Edge<V,E,W> > outgoing;  //stores outgoing edges
  vector<Edge<V,E,W> > allEdges=g->getEdges();
  int sum_edges=1;
  for (int i=0; i<allEdges.size(); i++){
    sum_edges+=allEdges[i].getWeight();
  }
  
  for(int i=0; i<vertices.size();i++){
    distances->insert(vertices[i],sum_edges);
    PQ->insert(sum_edges,vertices[i]);
  }
  distances->remove(src);
  distances->insert(src,0);
  PQ->updatePriority(0,src);
  while(!PQ->isEmpty()){
    u=PQ->removeMin();
    outgoing=g->getOutgoingEdges(u);
    for (int i=0;i<outgoing.size();i++){
      W uvCost=outgoing[i].getWeight();
      v=outgoing[i].getDest();
      if (distances->get(u)+uvCost<=distances->get(v)){
        distances->remove(v);
        distances->insert(v,distances->get(u)+uvCost);
        PQ->updatePriority(distances->get(u)+uvCost,v);
          
      }
    }
  }

  for(int i =0; i<vertices.size();i++){
    if(distances->get(vertices[i]) == sum_edges){//no path from src
      distances->remove(vertices[i]);
      distances->insert(vertices[i],-1);
    }
  }
  delete PQ;
  return distances;


}



