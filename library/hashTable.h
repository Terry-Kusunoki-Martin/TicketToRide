#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <string>
#include <vector>
#include "dictionary.h"
#include "pair.h"
#include "queue.h"
/**
 * Defines a HashTable, an implementation of the Dictionary interface that
 * stores data using a hash table.  Collisions in this hash table are resolved
 * with separate chaining.
 */
template <typename K, typename V>
class HashTable : public Dictionary<K,V> {
  private:
    std::vector<Pair<K,V> >* table;    // The hash table data, with
                                       // each entry in the array
                                       // storing a vector of key-value pairs
    int size;                          // The number of items currently
                                       // stored in the hash table.
    int capacity;                      // The current size of the
                                       // table array.
    double MAX_ALPHA;                  // Should expand hash table if
                                       // we exceed this load factor.
    int comparisons;                   // To help you report your statistics
    
  public:
    HashTable(int capacity = 53, double maxLoadFactor = 0.8);
                                       // Constructs hash table with default
                                       // capacity of 53, maxAlpha of 0.8.
    ~HashTable();
    void     insert(K key, V value);   // Inserts key and value into the hash
                                       // table, replacing the old value if
                                       // the key was already in the table.
    V        get(K key);               // Returns the value for a key in the
                                       // hash table, or throws runtime_error
                                       // if the key is not present.
    V        remove(K key);            // Removes a key-value pair from the
                                       // hash table, or throws runtime_error
                                       // if the key is not present.
    int      getSize();                // Returns number of items in table.
    bool     isEmpty();                // Returns if the table is empty.
    bool     containsKey(K key);       // Returns true if the hash table
                                       // contains the given key.
    bool     containsValue(V value);   // Returns true if the hash table 
                                       // contains the given value.
    std::vector<K> getKeys();          // Returns all keys in the table.
    std::vector<V> getValues();        // Returns all values in the table.
    std::vector<Pair<K,V> > getVector(K key);
    Queue<Pair<K,V> >* getItems();     // Returns a pointer to an iterator
                                       // for all K-V pairs in the table.
    void reportStats();                // Function to print out performance
                                       // of hash table

  private:
    int hash(int key, int capacity);        // Returns a hash code for the
                                            // given integer.
    int hash(std::string key, int capacity);// Returns a hash code for the
                                            // given string.
    double getLoadFactor();                 // Returns the current load factor
                                            // of the hash table.
    void expandCapacity();                  // Expands the capacity of the
                                            // hash table's array.
};

#include "hashTable-inl.h"

#endif
