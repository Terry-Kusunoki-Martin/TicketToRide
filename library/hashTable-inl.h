/* CS35 Lab 11
 * Terry Martin and Persis Ratouis
 *
 * hashTable-inl.h
 * defines the public and private functions of hashTable.h
 */ 

#include <string>
#include <stdexcept>
#include "arrayQueue.h"
#include <iostream>
#include <vector>

using namespace std;

//constructor - sets capacity and load factor with parameters - either user given or default
//              initializes comparisons, size, and table
template <typename K, typename V>
  HashTable<K,V>:: HashTable(int capacity, double maxLoadFactor){
        this->capacity = capacity; 
        this->MAX_ALPHA = maxLoadFactor;
        comparisons = 0;
        size = 0;
        this->table = new vector<Pair<K,V> >[capacity]; //a pointer to an arary of vectors
}


//destructor - clears the vector  
template <typename K, typename V>
  HashTable<K,V>::~HashTable(){
        delete [] table;
  }


/**
 * Inserts a new key-value pair into the hash table, or replaces the value
 * if the key is already in the table.
 */
template <typename K, typename V>
void HashTable<K,V>::insert(K key, V value) {
  int pos = hash(key, capacity);                 // Computes position of the 
                                                 // key in hash table array.
  
  for (int i = 0; i < table[pos].size(); ++i) {  // Replaces key-value pair
    comparisons++;                               // in the hash table
    if (table[pos][i].first == key) {             
      table[pos][i] = Pair<K,V>(key,value);
      return;
    }
  }

  ++size;                                        // Otherwise update the size
  table[pos].push_back(Pair<K,V>(key,value));    // and insert the item into
  if (getLoadFactor() > MAX_ALPHA) {              // the hash table, expanding
    expandCapacity();                            // the table if it exceeds
  }                                              // the max load factor.
}

/* get(K key) - determines if the key is in the table by 
 *              searching one vector (found from hashing key)
 *              throw an ERROR if the key is not in the table
 *              returns value associated with key
 *
 */ 
template <typename K, typename V>
V HashTable<K,V>::get(K key){
  int newKey = hash(key,capacity);  //computes position of the key in hash table array
                                    //a.k.a. vector containing key
  for(int i =0; i< table[newKey].size(); i++){ //loop through vector
    if(table[newKey][i].first == key){  //check if any keys match the passed-in key
      V found = table[newKey][i].second;
      return found;  //return the value associated with found key
    }
  }
  //throw an error if the key was not found
  throw std::runtime_error("In HashTable get(k): key not found\n");

}


/* remove(K key) - removes key from hash table; locates the key
 *                 by searching through one vector given by the hash
 *                 of key 
 *                 throws an ERROR if the key is not in the table
 *                 returns the value of the recently deleted key
 *
 */ 
template <typename K, typename V>
V HashTable<K,V>::remove(K key){
  int newKey = hash(key,capacity);  //computes position of the key in hash table array
                                    //a.k.a. vector containing key

    for(int i=0;i<table[newKey].size();i++){  //loop through vector 
      if(table[newKey][i].first == key){  //check if key is in vector
      V removed = table[newKey][i].second;  
      table[newKey].erase(table[newKey].begin()+i);  //remove vector 
                                                     //erase takes care of adjusting entries in vector after 
                                                     //one element has been removed
      size--;                   //decrement size
      return removed;           //return value assoicated with deleted key
    }
    }
    //throw an error if the key was not found
    throw std::runtime_error("In HashTable remove(k): key not found\n");
}

/* getSize - returns the size, or the number of KV pairs in the table
 *
 */ 

template <typename K, typename V>
int HashTable<K,V>::getSize(){
  return this->size;
}

/* isEmpty - returns true if there are no elements in the table
 *
 */ 
template <typename K, typename V>
bool HashTable<K,V>::isEmpty(){
  if(this->size == 0){
        return true;
  }     
  return false;
}

/* containsKey - checks if the key is in the table by looping through the 
 *              vector (found by hashing the key)
 *              returns true if found
 *
 */ 
template <typename K, typename V>
bool HashTable<K,V>::containsKey(K key){
  
  int newKey = hash(key,this->capacity);  //find the position of key in hash table
  for(int i = 0; i< this->table[newKey].size(); i++){  // loop through vector 
    if(table[newKey][i].first == key){  
        return true;
      }
  }
  return false;
}


/* containsValue - checks if the value is in the table by looping through 
 *                 all the vectors
 */ 
template <typename K, typename V>
bool HashTable<K,V>::containsValue(V value){
  
  for(int i = 0; i<this->capacity; i++){  //loop through hash table array
    for(int j = 0; j< this->table[i].size(); j++){  //loop down each vector
      if(table[i][j].second == value){  //check if value matches
        return true;
      }
    }
  }
  return false;
}

/* getKeys - returns a vector of all the keys in the entire table*/
template <typename K, typename V>
std::vector<K> HashTable<K,V>::getKeys(){
  std::vector<K> allKeys; //create vector of key type
  for(int i =0; i < this->capacity; i++){  //loop through hash table array
    for(int j = 0; j < this->table[i].size(); j++){ //loop through each vector
      allKeys.push_back(table[i][j].first);  //insert keys into allKeys
    }
  }
  return allKeys;  //return vector of keys
}

/* getValues - returns a vector of all the values in the entire table*/
template <typename K, typename V>
std::vector<V> HashTable<K,V>::getValues(){
  std::vector<V> allVals; //create vector of value type
  for(int i =0; i < this->capacity; i++){  //loop through hash table array
    for(int j = 0; j < this->table[i].size(); j++){ //loop down each vector 
      allVals.push_back(table[i][j].second); //insert values into allVals
    }
  }
  return allVals;  //return vector of values
}

/* getItems - returns a queue of all the K-V pairs */
template <typename K, typename V>
Queue<Pair<K,V> >* HashTable<K,V>::getItems(){
  Queue<Pair<K,V> >* listItems = new ArrayQueue<Pair<K,V> >;
  for(int i = 0; i< this->capacity; i++){  //loop through hash table array
    for(int j = 0; j<this->table[i].size(); j++){  //loop down vector 
        listItems->enqueue(table[i][j]);  //enqueue each Pair
    }
  }
  return listItems;  //return queue
}

/* reportStats - returns information about the hash table at a given state 
 *              percent full - based on capacity
 * */ 
template <typename K, typename V>
void HashTable<K,V>::reportStats(){
  double percentFull = getLoadFactor()*100; 
  cout << "Percent full : " << percentFull << "%." << endl;

  int max,members = 0;
  float sizeTotal,avgSize =0;
  
  for(int i =0; i<this->capacity; i++){  //loop through array
    if(this->table[i].size() > max){  //check for longest vector
      max = this->table[i].size();
    }
    if(this->table[i].size() != 0){  
      sizeTotal+=this->table[i].size();  //add all the sizes of the vectors 
      members++;                        //increment the number of non-zero vectors
    }
  }
  
  
  avgSize = sizeTotal/members;  // calculate the avgerage Size of the non-zero vectors
  
  cout << "Max size of any vector : " << max << endl;
  cout << "Avg size of non-empty vectors: " << avgSize << endl;

}

//////////////////////////////HELPER METHODS/////////////////////////////////

/*
 * Given a key and a hash table capacity, returns the hash code for that key.
 * This is a default implementation for mapping ints to a table index
 */
template <typename K, typename V>
int HashTable<K,V>::hash(int key, int capacity) {
  int rv = key % capacity;
  if (rv < 0) {      // Ensuring that our return value is positive, because
    rv += capacity;  // mod in C++ can return a negative value.
  }
  return rv;
}

/**
 * Given a key and a hash table capacity, returns the hash code for that key.
 * This maps strings to a table index
 */
template <typename K, typename V>
int HashTable<K,V>::hash(std::string key, int capacity) {
  int total = 0;
  for (int i = 0; i < key.length(); ++i) {
    total = 31*total + key[i];
  }
  int rv = total % capacity;
  if (rv < 0) {      // Ensuring that our return value is positive, because
    rv += capacity;  // mod in C++ can return a negative value.
  }
  return rv;
}

/* getLoadFactor - ratio between the size and the capacity of the hash table */
template <typename K, typename V>
double HashTable<K,V>::getLoadFactor(){
  double dSize = this->size;
  double dCap = this->capacity;
  return dSize/dCap;
}


/* expandCapcity 
 *      increases capacity by doubling it 
 *      creates new table with rehashed KV pairs with 
 *      new capacity
 */ 
template <typename K, typename V>
void HashTable<K,V>::expandCapacity(){
        int newCap = 2*this->capacity;  //double capacity

        //create a new hash table with a new capacity
        std::vector<Pair<K,V> >* newTable = new vector<Pair<K,V> >[newCap];
        
        for(int i = 0; i<this->capacity; i++){ // loop through current hash table array
          int vSize = this->table[i].size();  //size of each vector 
          
          for(int j = 0; j<vSize; j++){  //loop down vector
            
            Pair<K,V> temp = this->table[i][j];  //store KV pairs of vector one at a time
            int newHash = hash(temp.first,newCap);  //rehash the key of value in vector with new capacity
            newTable[newHash].push_back(temp);  //insert KV in newtable in rehash index;
          }
        }

        delete [] table;  //delete old table 
        this->table = newTable;  //point to the new table
        this->capacity = newCap;  // update capacity 
}
