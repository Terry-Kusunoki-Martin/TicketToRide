#ifndef STACK_H_
#define STACK_H_

/**
 * The Stack is a pure virtual (abstract) interface of a stack.  Stack
 * implementations should inherit from and implement this interface.
 */
template <typename T>
class Stack {
  public:
    virtual ~Stack() { };           // An empty virtual destructor.
    virtual void push(T item) = 0;  // Push a new item onto the stack.
    virtual T    pop() = 0;         // Remove and return the top item on stack.
    virtual int  getSize() = 0;     // Returns current number of items on stack.
    virtual bool isEmpty() = 0;     // Returns true if the stack is empty.
    virtual T    getTop() = 0;      // Returns (but does not remove) top item.
};

#endif  // STACK_H_
