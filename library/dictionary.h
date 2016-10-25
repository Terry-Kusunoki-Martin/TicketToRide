#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <vector>
#include "queue.h"
#include "pair.h"

/**
 * Dictionary defines a templated pure-virtual interface for a dictionary.
 * K is the key type, V the value type
 */
template <typename K, typename V>
class Dictionary {
 public:
  virtual ~Dictionary() {}

  virtual void insert(K key, V value) = 0;  // Inserts a key-value pair into
                                            // the dictionary. If they key
                                            // already exists, its value must
                                            // be replaced
  virtual V    get(K key) = 0;              // Returns the value for a key
                                            // in the dictionary.
  virtual V    remove(K key) = 0;           // Removes a key-value pair from
                                            // the dictionary, returning the
                                            // value for the key.
  virtual int  getSize() = 0;               // Returns the number of key-
                                            // value pairs in dictionary.
  virtual bool isEmpty() = 0;               // True if dictionary is empty.
  virtual bool containsKey(K key) = 0;      // Returns true if dictionary 
                                            // contains the given key.
  virtual bool containsValue(V value) = 0;  // Returns true if dictionary
                                            // contains the given value.
  virtual std::vector<K> getKeys() = 0;     // Returns a list of all keys
                                            // in the dictionary.
  virtual std::vector<V> getValues() = 0;   // Returns a list of all values
                                            // in the dictionary.
  virtual Queue<Pair<K,V> >* getItems() = 0;// Returns an iterator of all
                                            // key-value pairs in dictionary
  virtual void reportStats() = 0;               // To report performance
};

#endif
