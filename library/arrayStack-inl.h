// Implement an ArrayStack here.

#include <stdexcept>

#include "circularArrayList.h"
using namespace std;

template <typename T>
ArrayStack<T> :: ArrayStack(){
  //nothing to be done since CircularArrayList is initialized upon calling
  //the constructor
}

//deconstructor
template <typename T>
ArrayStack<T> :: ~ArrayStack(){
    //nothing to be done
}

//push: inserts new item at front of list
template <typename T>
void ArrayStack<T> :: push(T item){
        items.insertAtHead(item);
}

//pop: remove and return item at front of list
template <typename T>
T ArrayStack<T> :: pop(){
        return items.removeHead();
}

//getSize: returns size of list
template <typename T>
int ArrayStack<T> :: getSize(){
        return items.getSize();
}

//isEmpty:  returns 1 if empty, 0 if not
template <typename T>
bool ArrayStack<T> :: isEmpty(){
        return items.isEmpty();
}

//getTop: peeks at head of list without popping it off
template <typename T>
T ArrayStack<T> :: getTop(){
        return items.peekHead();
}
