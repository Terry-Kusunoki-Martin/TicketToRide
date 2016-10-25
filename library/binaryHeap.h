#ifndef BINARYHEAP_H_
#define BINARYHEAP_H_

#include "priorityQueue.h"
#include "pair.h"

/**
 * The BinaryHeap is an array-based heap implementation of a min PriorityQueue
 * for priority-type P and value-type V.  You don't need to modify this
 * definition, but you may if you wish to add additional private functions.
 */
template <typename P, typename V>
class BinaryHeap : public PriorityQueue<P,V> {
  private:
    Pair<P,V>* items;                  // The items currently stored in heap.
    int size;                          // Number of items currently in heap.
    int capacity;                      // The current capacity of items array.

  public:
    BinaryHeap();
    ~BinaryHeap(); 
                                   
    void insert(P priority, V value);  // Put an item into the heap.
    V    removeMin();                  // Remove and return value of min item.
    V    getMin();                     // Returns (but does not remove) min.
    P    getMinPriority();             // Returns min item's priority.
    void updatePriority(P priority, V value);
    int  getSize();                    // Returns current number of items.
    bool isEmpty();                    // Returns true if the heap is empty.

  private:
    void bubbleUp(int currentPos);     // Bubbles a position up in the heap to
                                       // restore the heap-order property.
    void bubbleDown(int currentPos);   // Bubbles a position down in the heap
                                       // to restore the heap-order property.
    int getLeft(int pos);              // Returns left pos for a given pos.
    int getRight(int pos);             // Returns right pos for a given pos.
    int getParent(int pos);            // Returns parent pos for a given pos.
    bool isLeaf(int pos);              // Returns if the position is a leaf.
    void expandCapacity();             // Expands the capacity of this heap.
};

#include "binaryHeap-inl.h"
//#include "binaryHeap-private-inl.h

#endif  // BINARYHEAP_H_
