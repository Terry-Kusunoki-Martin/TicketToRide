#include <assert.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "graph.h"
#include "ugraph.h"
#include "graph-algorithms.h"

using namespace std;

void SimpleGraph();
void SimpleUGraph();
void RemoveUGraph();
void testDFS();
void testBFS();
void testDijkstra();
void swatMap();

int main() {
  SimpleGraph();
  SimpleUGraph();
  RemoveUGraph();
  testBFS();
  testDFS();
  testDijkstra();
  swatMap();
  return 0;
}

void SimpleGraph(){
  Graph<char,string,int>* graph = new Graph<char,string,int>();

  graph->insertEdge('a','b',"a->b",5);
  graph->insertEdge('a','c',"a->c",5);
  graph->insertEdge('b','c',"b->c",6);
  graph->insertEdge('c','a',"c->a",6);
  graph->insertEdge('c','d',"c->d",5);
  vector<Edge<char,string,int> > edges = graph->getEdges();
  char src, dest;
  for(int i=0; i<edges.size(); i++){
    src = edges[i].getSrc();
    dest = edges[i].getDest();
    assert(graph->containsEdge(src,dest));    
  }

  delete graph;
}

void SimpleUGraph(){
  Graph<char,string,int>* graph = new UGraph<char,string,int>;

  graph->insertEdge('a','b',"a->b",5);
  graph->insertEdge('a','c',"a->c",5);
  graph->insertEdge('b','c',"b->c",6);
  graph->insertEdge('c','d',"c->d",5);
  vector<Edge<char,string,int> > edges = graph->getEdges();
  char src, dest;
  for(int i=0; i<edges.size(); i++){
    src = edges[i].getSrc();
    dest = edges[i].getDest();
    assert(graph->containsEdge(src,dest));    
  }

  delete graph;
}

void RemoveUGraph(){
 Graph<char,string,int>* graph = new UGraph<char,string,int>();

  graph->insertEdge('a','b',"a->b",5);
  graph->insertEdge('a','c',"a->c",5);
  graph->insertEdge('b','c',"b->c",6);
  assert(graph->containsEdge('b','c'));
  vector<Edge<char,string,int> > incoming = graph->getIncomingEdges('c');
  int sizeBefore = incoming.size();
  graph->removeEdge('b','c');
  assert(!graph->containsEdge('b','c'));
  incoming = graph->getIncomingEdges('c');
  assert(incoming.size() == sizeBefore-1);
  delete graph;
}



void testDFS(){
  Graph<char,string,int>* graph = new Graph<char,string,int>();
  graph->insertEdge('a','b',"a->b",5);
  graph->insertEdge('a','c',"a->c",5);
  graph->insertEdge('b','c',"b->c",6);
  graph->insertEdge('c','d',"c->d",5);
  graph->insertEdge('e','b',"e->b",4);
  assert(reachableDFS('a','c',graph));
  assert(reachableDFS('e','c',graph));
  assert(!reachableDFS('a','e',graph));
  delete graph;

}

void testBFS(){
  Graph<char,string,int>* graph = new Graph<char,string,int>();
  graph->insertEdge('a','b',"a->b",5);
  graph->insertEdge('a','c',"a->c",5);
  graph->insertEdge('b','c',"b->c",6);
  graph->insertEdge('c','d',"c->d",5);
  graph->insertEdge('e','b',"e->b",4);
  try{
    vector<char> BFS = shortestLengthPathBFS('a','e', graph);
    assert(false);
  }
  catch (std::runtime_error e){}

  vector<char> BFS = shortestLengthPathBFS('a','d', graph);
  assert(BFS.size()==3);
  assert(BFS[0]=='a');
  assert(BFS[1]=='c');
  assert(BFS[2]=='d');

  delete graph;

}

void testDijkstra(){
  Graph<char,string,int>* graph = new Graph<char,string,int>();
  graph->insertEdge('a','b',"a->b",5);
  graph->insertEdge('a','c',"a->c",5);
  graph->insertEdge('b','d',"b->d",3);
  graph->insertEdge('b','c',"b->c",6);
  graph->insertEdge('c','d',"c->d",5);
  graph->insertEdge('e','b',"e->b",4);
  Dictionary<char,int>* djTest= dijkstras('a',graph);
  vector<char> vertices =graph->getVertices(); 
  assert(djTest->get('b')==5);
  assert(djTest->get('c')==5);
  assert(djTest->get('d')==8);
  assert(djTest->get('e')==-1);
  
  delete djTest;
  delete graph;

}

void swatMap(){

  cout << "Let's build a map of Swarthmore!" << endl;
  Graph<string,string,int>* graph = new UGraph<string,string,int>();
  graph->insertEdge("softball","ville","softball->ville",1);
  graph->insertEdge("softball","soccer","softball->soccer",1);
  graph->insertEdge("softball","tunnel","softball->tunnel",1); 
  graph->insertEdge("softball","train","softball->train",1);
  graph->insertEdge("bridge","tennis","bridge->tennis",1);
  graph->insertEdge("bridge","Wharton","bridge->Wharton",1);
  graph->insertEdge("sharples","clothier","sharples->clothier",1);
  graph->insertEdge("sharples","tunnel","sharples->tunnel",1);
  graph->insertEdge("soccer","tennis","soccer->tennis",1);
  graph->insertEdge("Wharton","clothier","Wharton->clothier",1);
  graph->insertEdge("magill","parrish","magill->parrish",1);
  graph->insertEdge("magill","train","magill->train",1);
  graph->insertEdge("tennis","tunnel","tennis->tunnel",1);
  graph->insertEdge("parrish","clothier","parrish->clothier",1);
  graph->insertEdge("train","ville","train->ville",1);
  cout << "Enter a source and destination\n" << endl;
  string source,dest="";
    cin>>source;
    cin>>dest;

  cout << (*graph) << endl;
  if(reachableDFS(source,dest,graph)){
     cout << "This route is possible" << endl;
  }

  vector<string> BFS = shortestLengthPathBFS(source,dest,graph);
  cout << "The shortest path length is: " << BFS.size()-1 << endl;
  cout << "Using BFS:" <<endl;
  for(int i=0;i<BFS.size();i++){
        cout << BFS[i] << endl;
  }


  Dictionary<string,int>* djTest= dijkstras(source,graph);
  vector<string> vertices =graph->getVertices();

  for(int i=0;i<vertices.size();i++){
      cout <<vertices[i] << ": ";
      cout << djTest->get(vertices[i]) << endl;
    
  }
  delete djTest;
  delete graph;
}
