#include <stdexcept>  // For runtime_error

using namespace std;


/**
 * Constructs a binary heap with some arbitrary initial capacity.
 */
template <typename P, typename V>
BinaryHeap<P,V>::BinaryHeap() {
  size = 0;
  capacity = 10;
  items = new Pair<P,V>[capacity];
}


/**
 * Frees any memory allocated by this binary heap.
 */
template <typename P, typename V>
BinaryHeap<P,V>::~BinaryHeap() {
  delete [] items;
}

                                   
/**
 * Inserts a new priority-value pair into this binary heap.
 */
template <typename P, typename V>
void BinaryHeap<P,V>::insert(P priority, V value) {
  if(size==capacity){
    expandCapacity();
  }
  items[size].first = priority;
  items[size].second = value;
  bubbleUp(size);
  size++;
}


template <typename P, typename V>
void BinaryHeap<P,V>::updatePriority(P priority, V value) {
  for (int i =0;i<size;i++){
        if (items[i].second==value){
          P prevPriority=items[i].first;
          items[i].first=priority;
          if (prevPriority<priority){
            bubbleDown(i);
          }
          else{
            bubbleUp(i);
          }
          return;
        }
  }
  throw std::runtime_error("In Binary Heap::no matching value found.");
}

/**
 * Removes and returns the minimum item from this binary heap.  Throws a 
 * runtime_error if this binary heap is empty.
 */
template <typename P, typename V>
V BinaryHeap<P,V>::removeMin() {
  if(isEmpty()){
    throw std::runtime_error("removeMin called on empty BinaryHeap");
  }
  V toReturn = items[0].second;
  items[0].first = items[size-1].first;
  items[0].second = items[size-1].second;
  size--;
  bubbleDown(0);
  return toReturn;
}


/**
 * Returns but does not remove the value of the minimum item from this binary 
 * heap.  Throws a runtime_error if this binary heap is empty.
 */
template <typename P, typename V>
V BinaryHeap<P,V>::getMin() {
  if(isEmpty()){
    throw std::runtime_error("getMin called on empty BinaryHeap");
  }
  return items[0].second;
}


/**
 * Returns the priority of the minumum item in this binary heap.  Throws a
 * runtime_error if this binary heap is empty.
 */
template <typename P, typename V>
P BinaryHeap<P,V>::getMinPriority() {
  if(isEmpty()){
    throw std::runtime_error("getMinPriority called on empty BinaryHeap");
  }
  return items[0].first;
}


/**
 * Returns the number of items currently stored in this binary heap.
 */
template <typename P, typename V>
int BinaryHeap<P,V>::getSize() {
  return size;
}


/**
 * Returns true if this binary heap is currently empty, and false otherwise.
 */
template <typename P, typename V>
bool BinaryHeap<P,V>::isEmpty() {
  return size == 0;
}


/**
 * Given an integer position of an item in the heap's array, restores the heap
 * order by recursively bubbling up the item at that position into the 
 * correct position in the heap.
 */
template <typename P, typename V>
void BinaryHeap<P,V>::bubbleUp(int currentPos) {
  if(currentPos == 0){ //stop if at right
    return;
  }

  int parentPos = getParent(currentPos);
  if(items[currentPos].first < items[parentPos].first){
    Pair<P,V> temp = items[currentPos];
    items[currentPos] = items[parentPos];
    items[parentPos] = temp;
    bubbleUp(parentPos);
  }
}


/**
 * Given an integer position of an item in the heap's array, restores the heap
 * order by recursively bubbling down the item at that position into the 
 * correct position in the heap.
 */
template <typename P, typename V>
void BinaryHeap<P,V>::bubbleDown(int currentPos) {
  if(isLeaf(currentPos)){
    return;
  }
  int minChild = getLeft(currentPos);
  int right = getRight(currentPos);
  if(right < size && items[minChild].first > items[right].first){
    minChild = right;
  }
  if(items[currentPos].first > items[minChild].first){
    Pair<P,V> temp = items[currentPos];
    items[currentPos] = items[minChild];
    items[minChild] = temp;
    bubbleDown(minChild);
  }
}


/**
 * Given an integer position of an item in the heap's array, returns the
 * position of that item's left child in the heap.  The position of the
 * left child is always returned, even if the left child does not currently
 * contain an item.
 */
template <typename P, typename V>
int BinaryHeap<P,V>::getLeft(int pos) {
  return 2*pos+1;
}


/**
 * Given an integer position of an item in the heap's array, returns the
 * position of that item's right child in the heap.  The position of the
 * right child is always returned, even if the right child does not currently
 * contain an item.
 */
template <typename P, typename V>
int BinaryHeap<P,V>::getRight(int pos) {
  return 2*pos+2;
}


/**
 * Given an integer position of an item in the heap's array, returns the
 * position of that item's parent in the heap. 
 */
template <typename P, typename V>
int BinaryHeap<P,V>::getParent(int pos) {
  return (pos-1)/2;
}


/**
 * Given an integer position of an item in the heap's array, returns true
 * if that item is a leaf in the heap, and false otherwise.
 */
template <typename P, typename V>
bool BinaryHeap<P,V>::isLeaf(int currentPos) {
  return getLeft(currentPos) >= size;
}


/**
 * Expands the capacity of the array storing the heap, to store more items
 * when needed.
 */
template <typename P, typename V>
void BinaryHeap<P,V>::expandCapacity() {
  capacity *= 2;

  Pair<P,V>* newItems = new Pair<P,V>[capacity];
  for(int i = 0; i < size; i++){
    newItems[i] = items[i];
  }
  delete [] items;
  items = newItems;
}

