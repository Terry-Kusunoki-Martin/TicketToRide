/*ticketToRide: This file is our implementation for the Ticket To
 * Ride Game.
 *
 * Terry Kusunoki-Martin and Persis Ratouis
 */

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include "ugraph.h"
#include "graph-algorithms.h"
#include "library/pair.h"
#include "ticket.h"
#include "library/dictionary.h"
#include "library/circularArrayList.h"
#include <iostream>
using namespace std;
typedef Graph<string,string,int> TTRMap; //To simplify syntax

//main functions here
void printAndDisplay(TTRMap* g);
void drawTickets(vector<Ticket>* hand, Queue<Ticket>* deck, TTRMap* g);
void claimRoute(Pair<vector<Pair<string, string> >,vector<Pair<string,string> > >* routes, int p, TTRMap* g,vector<Pair<string,int> >* trainHand);
void printTicketInfo(vector<Ticket>* hand, Pair<vector<Pair<string, string> >, vector<Pair<string,string> > >* routes, TTRMap* g);
void calculateScore(Pair<vector<Pair<string, string> >, vector<Pair<string,string> > >* routes, vector<Ticket>* hand, TTRMap* g);
void drawTrain(vector<Pair<string,int> >* trainHand, Stack<string>* trainDeck);
void printTrainCards(vector<Pair<string,int > >* trainHand);

//helper methods
TTRMap* loadMapFromFile(string mapfile);
TTRMap* createPlayerGraph(vector<Pair<string,string> > cities,TTRMap* g);
Queue<Ticket>* loadTicketsFromFile(string ticketfile, TTRMap* g);
Stack<string>* createTrainDeck(TTRMap* g,vector<Pair<string,int> >* trainHand);
int getMenuChoice();
int findWeight(string src, string dest, TTRMap* g);
int conversion(int edgeweight);
void printMyRoutes(Pair<vector<Pair<string, string> >, vector<Pair<string,string> > >* routes, TTRMap* g);
void printAvailableRoutes(Pair<vector<Pair<string, string> >,vector<Pair<string,string> > >* routes, TTRMap* g,bool claimable, vector<Pair<string,int> >* trainHand);
bool checkCitySpell(string city, vector<string> cities);
void printTickets(CircularArrayList<Ticket>* store, TTRMap* g);

int main(int argc, char *argv[]) {

  if(argc != 3) {//check cmdline
    cerr << "Incorrect number of arguments" << endl;
    cerr << "Usage: ticketToRide map-file ticket-file" << endl;
    return 1;
  }
  //we need to use an RNG at several points in this game
  srand(time(NULL));
  
  //here we initialize the map, deck of tickets, deck of train cards, and
  //the players' hand for each
  TTRMap* TTR = loadMapFromFile(argv[1]);
  Queue<Ticket>* deck = loadTicketsFromFile(argv[2],TTR);
  vector<Pair<string,int> >* trainHand = new vector<Pair<string, int> >;
  Stack<string>* trainDeck = createTrainDeck(TTR,trainHand);
  vector<Ticket>* hand=new vector<Ticket>;
  
  cout << *TTR << endl;//print out the map
  drawTickets(hand,deck,TTR);//force player to draw tickets at the beginning
  //routes is a pair of vector of string pairs. The first vector keeps track of the
  //routes the user(Player 1) has claimed, while the second vector keeps track of the
  //routes other players have claimed
  Pair<vector<Pair<string, string> >,vector<Pair<string,string> > >* routes = new Pair<vector<Pair<string,string> >, vector<Pair<string,string> > >;
  
  int choice = getMenuChoice();//get user input and act accordingly
  
  while(choice != 0){     
    
    if(choice == 1){
      printAndDisplay(TTR);
    }else if(choice == 2){
      printMyRoutes(routes,TTR);
    }else if(choice == 3){
      printAvailableRoutes(routes,TTR,1,trainHand);
    }else if(choice == 4){
      printAvailableRoutes(routes,TTR,0,trainHand);
    }else if(choice == 5){
      claimRoute(routes,1,TTR,trainHand);
    }else if(choice == 6){
      claimRoute(routes,2,TTR,trainHand);
    }else if(choice == 7){
      drawTickets(hand,deck,TTR);
    }else if(choice == 8){
      drawTrain(trainHand,trainDeck);
    }else if(choice == 9){
      printTicketInfo(hand,routes,TTR);
    }else if(choice == 10){
      printTrainCards(trainHand);
    }else if(choice == 11){
      calculateScore(routes,hand,TTR);
    }
    choice = getMenuChoice();

  }

  //delete all variable allocated on the heap
  delete hand;
  delete routes;
  delete TTR;
  delete deck;
  delete trainHand;
  delete trainDeck;
  return 0;
}

/* Loads the game board from file into a Graph object
 * @param mapfile - location of file defining the game board
 * @return TTRMap* - dynamically allocated graph object with
 *                   cities (vertex) and routes (edges)
 * @error throws runtime_error if file does not exist
 * The caller is responsible for cleaning up dynamically allocated memory
 */

TTRMap* loadMapFromFile(string mapfile) {
  //The map is an undirected graph
  TTRMap *graph = new UGraph<string,string,int>;
  cout<< mapfile << endl;
  ifstream input(mapfile.c_str());
  if(input.is_open() == false){
    throw runtime_error("File does not exist for loadMapFromFile");
  }

  
  string edge;
  int next, prev;
  string src, dest, rest;
  int length;
  
  /*getline obtains an entire line as a string.  This loop parses one line
   * in the format:
   *   vertexA : vertexB
   * and defines an undirected edge between vertexA and vertexB
   */
  while(getline(input, edge)) {
    prev = 0;

    next = edge.find_first_of(':', prev);
    src = edge.substr(0, next);
    prev = next+1;
    
    next = edge.find_first_of(':', prev);
    dest = edge.substr(prev, next-prev);
    prev = next+1;
    
    rest = edge.substr(prev);
    length = atoi(rest.c_str());


    graph->insertEdge(src, dest, src + " -- " + dest, length);
  }
  return graph;
}

/*loadTicketsFromFile parses the given file in order to create a deck of
  tickets.
  Returns:a shuffled queue of all the tickets read in from the file.
  */
Queue<Ticket>* loadTicketsFromFile(string ticketfile,TTRMap* g){
  Queue<Ticket>* deck = new ArrayQueue<Ticket>;
  vector<Ticket> intermediate;
  HashTable<string,Ticket> tickets;

  ifstream input(ticketfile.c_str());
  if(input.is_open() ==false){//try to open file
    throw runtime_error("File does not exist for loadTicketFromFile");
  }
  
  string edge, src, dest;
  int next,prev,points;
  Ticket card;

  while(getline(input,edge)){//use getline to parse file
    prev=0;
    
    next = edge.find_first_of(':', prev);
    src = edge.substr(0, next);
    prev = next+1;
    
    dest = edge.substr(prev);
    points = findWeight(src,dest,g); 

    card.setCities(src,dest);
    card.setPoints(points);
    tickets.insert(edge,card);//insert each ticket into hashtable
    //we used a hashtable here because it will insert the tickets in
    //an arbitrary order
  }

  intermediate = tickets.getValues();//retrieve tickets from hash table
  
  for(int i=0;i<intermediate.size();i++){//add them to the deck
        deck->enqueue(intermediate[i]);
  }
  return deck;
}
/*findWeight is a helper method that, given a src and a dest on the TTRMap,
  returns the shortest distance between them.
  */
int findWeight(string src, string dest, TTRMap* g){
  Dictionary<string,int>* path = dijkstras(src,g);
  int shortestPath = path->get(dest);
  delete path;
  return shortestPath;
}

/*createTrainDeck initializes the deck of train cards.
  inputs:none
  returns:deck of train cards of colors red, green, and blue.
  */
Stack<string>* createTrainDeck(TTRMap* g,vector<Pair<string,int> >* trainHand){
  vector<Edge<string,string,int> > edges = g->getEdges();
  Stack<string>* trainDeck = new ArrayStack<string>;
  vector<Pair<string,int> > colors;
  string color;

  int size,random = 0;

  //we made the size of the deck be equal to the sum of all the edges in the graph
  for(int i = 0; i < edges.size(); i++){
     size+=edges[i].getWeight();
  }
  
  //initialize the capacity for each color card
  int cap_red = size/3;
  int cap_blue=size/3;
  int cap_grn=size-2*size/3;//account for integer division

  //push back each color-size pair onto colors
  colors.push_back(Pair<string,int>("red",cap_red));
  colors.push_back(Pair<string,int>("blue",cap_blue));
  colors.push_back(Pair<string,int>("green",cap_grn));

  
  for(int i=0;i<size;i++){
    random=rand()%(colors.size());//pick a random color
    if(colors[random].second > 0){//if there are still cards of this color left
    trainDeck->push((colors[random].first));//push this color onto the deck
    colors[random].second--;//decrement number of this color left
    }else{//no more cards left of this color
      colors.erase(colors.begin() + random);
    }
  }

  //we also give the player four train cards to start out with.
  //The player's hend consists of 3 pairs.  Each pair is the color,
  //and the number of train cards of that color the hand has
  trainHand->push_back(Pair<string,int>("red",0));
  trainHand->push_back(Pair<string,int>("blue",0));
  trainHand->push_back(Pair<string,int>("green",0));
  for(int i=0;i<4;i++){
    color=trainDeck->pop();
    for (int j=0;j<3;j++){
      if ((*trainHand)[j].first==color){
        (*trainHand)[j].second++;
      }
    }
  }

  return trainDeck;

}
/* Displays the main menu and obtains a legal choice from the user
 * @return int indicating the user's choice between 0 and 8
 */
int getMenuChoice(){
  int i = 0;
  cout << "\nMenu:\n";
  cout << "\t(" << i++ << ") Exit\n";
  cout << "\t(" << i++ << ") Print/display game board\n";
  cout << "\t(" << i++ << ") Print/display my routes\n";
  cout << "\t(" << i++ << ") Print routes you have the capability to claim\n";
  cout << "\t(" << i++ << ") Print available routes\n";
  cout << "\t(" << i++ << ") Claim route for self\n";
  cout << "\t(" << i++ << ") Claim route for opponent\n";
  cout << "\t(" << i++ << ") Draw and select tickets\n";
  cout << "\t(" << i++ << ") Draw and select train cards\n";
  cout << "\t(" << i++ << ") Print tickets and their status\n";
  cout << "\t(" << i++ << ") Print your current train cards\n";
  cout << "\t(" << i++ << ") Calculate current score\n\n";
  
  int choice = -1;
  while(choice < 0 || choice >= i){
    cout << "Enter choice: ";
    cin >> choice;
  }
  cin.ignore(); //Removes new line character in case you want to use getline
  cout << endl;

  return choice;
}

/* Prints and displays map in neato format using graphViz package
 * @param g - a Graph object
 * @error throws runtime_error if g is NULL
 */
void printAndDisplay(TTRMap *g){
  if(g == NULL){
    throw runtime_error("printAndDisplay called on undefined graph");
  }
  cout << (*g) << endl;
  /*This displays a graph.  This will not work on an outside system so
   * comment these four lines out if you are working from home */
  g->graphViz("out.neato");
  system("neato -Tpng -o out.png out.neato");
  system("xv out.png");
  system("rm -f out.png out.neato");
}

/*drawTickets: Adds tickets to the user's hand.  Makes sure the user draws
    at least one ticket(or 2 if the first time).
    */
void drawTickets(vector<Ticket>* hand, Queue<Ticket>* deck, TTRMap* g){
  int counter=0;
  CircularArrayList<Ticket>* storetickets = new CircularArrayList<Ticket>;
  if (deck->isEmpty()){
    cout << "No more cards in the deck!\n";
    return;
  }
  while(!deck->isEmpty() && counter<3){//pick top 3 cards, or less is deck has less than that
    storetickets->insertAtTail(deck->dequeue());
    counter++;
  }
  
  bool done = false;
  
  int keep = -1;
  int picked = 0;
  while(done == false){
    printTickets(storetickets, g);
    cout << "\nEnter choice or 0 to discard remaining tickets: ";
    while(keep < 0 || keep > counter-picked){//make sure user inputs num 
                                             //in correct range
      cin >> keep; 
      if (keep < 0 || keep > counter-picked){
        cout<<"Enter a number between 1 and "<< counter-picked <<": ";
      }
      while(keep == 0 && (hand->size()<2 || picked == 0)){
        if(hand->size()<2){//make sure they end up with at least 2 cards in hand
          cout << "You must select at least " << 2-hand->size() << " more ticket(s):  ";
        }else{//if we get here, the user has not picked any cards
          cout << "You must select at least one ticket:  ";
        }
        cin >> keep;
      }
    }
    //now that we have a valid input, we can act on it
    if(keep!=0){
      hand->push_back(storetickets->get(keep-1));
      storetickets->remove(keep-1);
      picked++;
      if (picked==counter){//cards exhausted
        cout<<"No more cards to pick!\n";
        done=true;
      }
    }
    else{
      done=true;
    }
    keep=-1;//reset cin variable to out of bounds
  }
  
  
  for(int i=0;i<storetickets->getSize();i++){//put remaining tickets back into deck
    if(storetickets->get(i).getPoints() != -2){
      deck->enqueue(storetickets->get(i));
    }
  }
  delete storetickets;
}

/*printTickets is a helper function to drawTickets that just outputs
    info about each ticket.
    */
void printTickets(CircularArrayList<Ticket>* store, TTRMap* g){

  cout << "Select a ticket to add to your hand:" << endl;
  string src, dest;
  for(int i = 0; i < store->getSize(); i++){
    src = store->get(i).getCities().first;
    dest = store->get(i).getCities().second;
    if(store->get(i).getPoints() != -2){
     cout << i+1 << ") ";
     cout<< src << " to " << dest << " for " << store->get(i).getPoints() << " points\n";


     if(reachableDFS(src,dest,g)){//there's still a path to dest 
      printf("%16s\n","Achieveable");
      
      //now run dijkstra's and BFS to get shortest length/cost paths
      vector<string> shortestpath = shortestLengthPathBFS(src,dest,g);
      Dictionary<string, int>* leastcost = dijkstras(src,g);
      int cost = leastcost->get(dest);
    
      printf("%44s%lu\n","Fewest routes needed (original board): ",shortestpath.size()-1);
      printf("%47s%d\n\n","Fewest train cars needed (current board): ",cost);
    
      delete leastcost;

     }else{
       printf("%20s\n","Not achieveable");
     }
   
    }
  }
}

/*claimRoute claims a route for either the user or an opponent.
    the int parameter p determines whether the route is being claimed
    for the user or for an opponent.  If the player claiming the route
    is an opponent, the route is deleted from the graph.

    side effects: updates the tracking variable routes
    */
void claimRoute(Pair<vector<Pair<string, string> >,vector<Pair<string,string> > >* routes, int p, TTRMap* g, vector<Pair<string,int> >* trainHand){

  string cityA,cityB;
  vector<string> cities = g->getVertices();
  int biggestSet,index=0;

  //here we find the biggest unicolor set of cards the user has, since
  //that is a bottleneck for how large a route they can claim
  for (int i=0;i<(*trainHand).size();i++){
    if((*trainHand)[i].second>biggestSet){
      biggestSet=(*trainHand)[i].second;
      index=i;
    }
  }

  cout << "Enter city: ";
  getline(cin, cityA);
  
  //here we do some error checking, and exit if there is an issue
  if(!checkCitySpell(cityA,cities)){//if cityA is in the graph
     cout << "Error: that city does not exist. Returning to main menu." << endl;
     return;
  }
  cout << "enter city: ";
  getline(cin, cityB);
  if(!checkCitySpell(cityB,cities)){//if cityB is in the graph
     cout << "Error: that city does not exist. Returning to main menu." << endl;
     return;
  }

  for(int i=0;i<routes->first.size(); i++){//see if user already claimed
        if(routes->first[i].first == cityA && routes->first[i].second == cityB){
          cout << "Error: You have already claimed that route. Returning to main menu." << endl;
          return;
        }
  }
  for(int i=0; i<routes->second.size();i++){//see is other player has claimed
        if(routes->second[i].first == cityA && routes->second[i].second == cityB){
          cout << "Error: That route has been claimed by another player. Returning to main menu." << endl;
          return;
        }
  }
  
  if(!(g->containsEdge(cityA,cityB))){//make sure A->B is a valid route
        cout << "Error: That is not a valid route. Returning to main menu." << endl;
        return;
  }

  int cost = findWeight(cityA,cityB,g);
  if(cost>biggestSet){//user cannot claim route if not enough train cards
        cout << "You do not have sufficient train cards to claim that route!\nYou have " << biggestSet << " train cards of the same color, but you need " << cost << ".\n";
        return;
  }

  if(p==1){  //self
    //update routes
    routes->first.push_back(Pair<string,string>(cityA,cityB));
    routes->first.push_back(Pair<string,string>(cityB,cityA));
    
    cout << "You successfully claimed this route for " << conversion(cost) << " points." << endl;
    //update hand of train cards
    (*trainHand)[index].second-=cost;
  }else if(p == 2){   //oponent
    //update routes
    routes->second.push_back(Pair<string,string>(cityA,cityB));
    routes->second.push_back(Pair<string,string>(cityB,cityA));
    
    cout << "Your opponent successfully claimed the route for " << conversion(cost) << " points." << endl;
    //remove edge from graph
    g->removeEdge(cityA,cityB);
  }

}

//checkCitySpell: helper method that makes sure a city as entered 
//is on the map
//returns:true if the city is on the map, false otherwise
bool checkCitySpell(string city, vector<string> cities){
   for(int i =0; i< cities.size();i++){
     if(cities[i] == city){
        return true;
     }
   }
   return false;
}

/*printMyRoutes prints out the user's claimed routes.
    returns: none
    */
void printMyRoutes(Pair<vector<Pair<string, string> >, vector<Pair<string,string> > >* routes, TTRMap* g){
  if(routes->first.size() == 0){
    cout << "You haven't claimed any routes yet.\n";  
  }
  for(int i = 0; i< routes->first.size(); i++){
    cout << routes->first[i].first << ":  {" << routes->first[i].second << "}\n";
  }
}

/*printAvailibleRoutes prints routes unclaimed by opponents
    if the bool claimable is true, the user wants to display only routes
    the user can claim with his/her train cards
    returns: none
    */
void printAvailableRoutes(Pair<vector<Pair<string, string> >, vector<Pair<string,string> > >* routes, TTRMap* g, bool claimable,vector<Pair<string,int> >* trainHand){
  vector<Edge<string,string,int> > edges = g->getEdges();
  cout << "Available Routes" << endl;
  cout << "------------------\n\n" << endl;
  string src, dest;
  int cars;
  bool skip;
  int biggestSet=0;
  //find largest set of train cards user has
  for (int i=0;i<3;i++){
    if((*trainHand)[i].second>biggestSet){
      biggestSet=(*trainHand)[i].second;
    }
  }
  
  //loop through every edge in the graph
  for(int i=0; i<edges.size(); i++){
    src = edges[i].getSrc();
    dest = edges[i].getDest();
    cars = edges[i].getWeight();
    
    Pair<string,string> sd(src,dest);
    Pair<string,string> ds(dest,src);
    
    for(int j =0; j< routes->first.size(); j++){
      //skip printing this route if user has already claimed it
      if((routes->first[j] == sd) || (routes->first[j] == ds)){
        skip=true;
      }
    }
    //if the route is longer than the user has resources for
    if (claimable && cars>biggestSet){
      skip=true;
    }
    if (!skip){//only print if skip is false
      printf("\t%s -- %s : %d Train Cars\n",src.c_str(),dest.c_str(),cars);
    }
    skip=false;//reset skip
  }
}

/*printTicketInfo: prints information about the user's held tickets
    returns: none
    */
void printTicketInfo(vector<Ticket>* hand, Pair<vector<Pair<string, string> >, vector<Pair<string,string> > >* routes, TTRMap* g){
  
  //create graph of only routes the user has claimed
  TTRMap* usergraph= createPlayerGraph(routes->first,g);
  
  cout << "Your current destination tickets and their status" << endl;
  string src,dest;
  int edgecost,sum=0;

  for(int i = 0; i<hand->size();i++){
    //get basic info about Ticket
    Pair<string,string> cities = (*hand)[i].getCities();
    int points = (*hand)[i].getPoints();
    //print source, dest, and points
    cout << i+1 << ") " << cities.first << " to " << cities.second << " for " << points << " points:\n";
    //if graph of user's routes connects the two cities, ticket is punched
    if(usergraph->containsVertex(cities.first) && usergraph->containsVertex(cities.second) && 
    reachableDFS(cities.first,cities.second,usergraph)){
        cout << "\tTicket punched.\n\n";
    }
    //otherwise, check the larger graph of availible routes
    else if(reachableDFS(cities.first,cities.second,g)){
        cout << "\tTicket not punched but path still exists." << endl;
        vector<string> cost = shortestLengthPathBFS(cities.first,cities.second,g);
        for(int j=0;j<cost.size()-1;j++){
          src=cost[j];
          dest=cost[j+1];
          edgecost=findWeight(src,dest,g);
          if(!usergraph->containsEdge(src,dest)){
            //add to cost user still needs if they haven't claimed this
            //particular route yet
            sum+=edgecost;
          }
        }
        cout << "\tYou still need at least " << sum << " cars to complete this path.\n\n";
        sum=0;
    }
  else{
      cout << "\tIt is impossible to punch this ticket.\n\n";
    }
  }
  delete usergraph;
}

/*createPlayerGraph creates a graph out of the routes the user has claimed
    returns: graph of user claimed routes
    */
TTRMap* createPlayerGraph(vector<Pair<string,string> > cities,TTRMap* g){
  TTRMap* playerG = new UGraph<string,string,int>;
  string src, dest;
  int weight;
  for(int i=0;i<cities.size();i+=2){//the +2 is to skip duplicate edges
    src = cities[i].first;
    dest = cities[i].second;
    weight = findWeight(src,dest,g);
    playerG->insertEdge(src, dest, src + " -- " + dest, weight);
  }
  return playerG;
}

/*calculateScore prints out a breakdown of the user's current score
  inputs:routes, user's tickets, graph
  */

void calculateScore(Pair<vector<Pair<string, string> >, vector<Pair<string,string> > >* routes, vector<Ticket>* hand, TTRMap* g){
  cout << "Your current score breakdown\n";
  cout << "----------------------------\n";

  cout << "Routes: " << endl;
  int totalScore,edgecost = 0;
  string src,dest;
  vector<Pair<string,string> > hasbeen;
  bool dontPrint,sign=false;
  TTRMap* player_graph=createPlayerGraph(routes->first,g);


  for(int i =0;i<routes->first.size();i++){//for each route the player claimed
    src = routes->first[i].first;
    dest = routes->first[i].second;
    edgecost = conversion(findWeight(src,dest,g));//point conversion done here

    if(hasbeen.size() == 0){//if nothing in vector yet
      printf("%25s -- %s: +%d points\n",src.c_str(),dest.c_str(),edgecost);
      hasbeen.push_back(Pair<string,string>(src,dest));
      hasbeen.push_back(Pair<string,string>(dest,src));
      totalScore+=edgecost;//add to score
    }else{
      for(int j = 0; j< hasbeen.size(); j++){//don't print if already
                                             //printed
        if(hasbeen[j] == Pair<string,string>(src,dest)){
          dontPrint=true;
        }
      }
      if(!dontPrint){
        printf("%25s -- %s: +%d points\n",src.c_str(),dest.c_str(),edgecost);
        hasbeen.push_back(Pair<string,string>(src,dest));
        hasbeen.push_back(Pair<string,string>(dest,src));
        totalScore+=edgecost;//add to score
      }
    }
    dontPrint=false;//reset dontPrint
  }  

  //calculate score from tickets
  cout << "\nDestination tickets:" << endl;
  for(int i =0; i<hand->size();i++){
    src = (*hand)[i].getCities().first;
    dest = (*hand)[i].getCities().second;
    edgecost = (*hand)[i].getPoints();
    
    if (player_graph->containsVertex(src) && player_graph->containsVertex(dest)){
      sign = reachableDFS(src,dest,player_graph);
    }
    if(!sign){//user has not punched ticket
      edgecost = edgecost*(-1);//add negative to point value
    }
    printf("%25s -- %s: %d points\n",src.c_str(),dest.c_str(),edgecost);
    totalScore+=edgecost;//add to points
    sign=false;//reset sign
  }
  //print total points
  cout << "\n Total: " << totalScore << " points." << endl;
  delete player_graph;
}

/*conversion: a helper function that converts a route's weight into a point
    value based on the table given.
  inputs: edge weight
  returns: point value
  */
int conversion(int edgeweight){
  if (edgeweight==1){
    return 1;
  }
  else if (edgeweight==2){
    return 2;
  }
  else if (edgeweight==3){
    return 4;
  }
  else if (edgeweight==4){
    return 7;
  }
  else if (edgeweight==5){
    return 10;
  }
  else{
    return 15;
  }
  
}

/*drawTrain is a function that lets a player draw two train cards.
  inputs: player's currently held train cards, deck of train cards
  returns:none
  */
void drawTrain(vector<Pair<string,int> >* trainHand, Stack<string>* trainDeck){
  vector<string> out;
  int counter=0;
  int draw,i1,i2;
  string train1,train2;
  //print user's current hand
  printTrainCards(trainHand);

  if (trainDeck->isEmpty()){
    cout << "No more cards in the deck!\n";
    return;
  }
  while(!trainDeck->isEmpty() && counter<5){
    out.push_back(trainDeck->pop());
    //here we see the advantage of using a Stack. If we always show the
    //5 cards on the top of the deck, and the user decides to draw from the
    //deck rather than the 5 cards that are out, the next time we pop 5 cards
    //from the top of the deck, it will be the same 5 that were out before
    counter++;
  }
  //display top 5 cards
  cout << "Currently face-up cards: \n\n";
  for(int i=0;i<counter;i++){
    cout<< "\t" << i << ") " << out[i] << "\n";
  }

  //ask if user wants to draw from the deck, or from the displayed cards
  cout<<"\nWould you like to draw from the deck(0), or pick from the face up cards(1)?\n";
  cin>>draw;
  while(draw!=0 && draw!=1){
    cout<<"\nPlease enter 0 to draw from the deck, or 1 to take from the face-up cards.\n";
    cin>>draw;
  }
  if (draw==0){//user want to draw from the deck
    train1=trainDeck->pop();
    train2=trainDeck->pop();
    cout << "You drew a " << train1 << " train, and a " << train2 << " train.\n";
    
  }
  else{//user wants to pick from the cards that are already out
    cout << "Choose two of the face-up cards: \n";
    cin >> i1;
    cin >> i2;
    while(i1<0 || i1>=counter || i2<0 || i2>=counter || i1 ==i2){
      cout<<"\nPlease enter two options within the range of those given (no repeats!).\n";
      cin >> i1;
      cin >> i2;
    }
    train1=out[i1];
    train2=out[i2];
  }
  
  //add train cards to user's hand
  for (int j=0;j<(*trainHand).size();j++){
    if ((*trainHand)[j].first==train1){
      (*trainHand)[j].second++;
    }
    if ((*trainHand)[j].first==train2){
      (*trainHand)[j].second++;
    }
  }  
}
/*printTrainCards: prints cards the user has in his hand
  inputs:user's hand of train cards
  */
void printTrainCards(vector<Pair<string,int > >* trainHand){
  if(trainHand->size() == 0){
     cout << "You don't have any train cards." << endl;
  }else{
    for(int i =0;i<trainHand->size();i++){
      cout << "You have " << (*trainHand)[i].second << " " << (*trainHand)[i].first << " cards." << endl;
    }
  }
}
