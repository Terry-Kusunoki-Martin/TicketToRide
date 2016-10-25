

template <typename T>
ArrayQueue<T>::ArrayQueue() { /* do nothing */ }

template <typename T>
ArrayQueue<T>::~ArrayQueue() { /* do nothing */ }

template <typename T>
void ArrayQueue<T>::enqueue(T item) {
  items.insertAtTail(item);
}

template <typename T>
T ArrayQueue<T>::dequeue() {
  return items.removeHead();
}

template <typename T>
int ArrayQueue<T>::getSize() {
  return items.getSize();
}

template <typename T>
bool ArrayQueue<T>::isEmpty() {
  return items.isEmpty();
}

template <typename T>
T ArrayQueue<T>::getFront() {
  return items.peekHead();
}


