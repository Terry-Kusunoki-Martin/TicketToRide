// Terry Kusunoki-Martin and Persis Ratouis
// CS35 Lab 6


#ifndef ARRAYSTACK_H_
#define ARRAYSTACK_H_

#include "stack.h"
#include "circularArrayList.h"

/**
 * ArrayStack is a circularArrayList-based implementation of the Stack 
 * interface.  It should support O(1) time performance for all operations.
 */
template <typename T>
class ArrayStack : public Stack<T> {
  private:
    CircularArrayList<T> items;

  public:
    ArrayStack();
    ~ArrayStack();

    void push(T item);
    T pop();
    int getSize();
    bool isEmpty();
    T getTop();
};

#include "arrayStack-inl.h"

#endif
