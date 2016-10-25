#ifndef PRIORITYQUEUE_H_
#define PRIORITYQUEUE_H_

/**
 * The PriorityQueue is a pure virtual (abstract) interface of a minimum
 * priority queue for priority-type P and value-type V.  Priority queue 
 * implementations should inherit from and implement this interface.  
 */
template <typename P, typename V>
class PriorityQueue {
  public:
    virtual ~PriorityQueue() { };      
    
    /* insert - adds an item with given priority and value to the PQ
     * @param priority - key for the item which determines its ordering
     * @param value - value to store with the given priority
     */
    virtual void insert(P priority, V value) = 0;

    virtual void updatePriority(P priority, V value) = 0;
    
    /* removeMin - removes the item with minimum priority and returns its value
     * @return V, value of removed item
     * @error throws runtime_error if there are no items in the PQ
     */
    virtual V    removeMin() = 0;     

    /* getMin - returns the value of the item with minimum priority
     * @return V, value of item with minimum priority
     * @error throws runtime_error if there are no items in the PQ
     */
    virtual V    getMin() = 0;         

    /* getMinPriority - returns the minimum priority stored in the PQ
     * @return P, the minimum priority
     * @error throws runtime_error if there are no items in the PQ
     */
    virtual P    getMinPriority() = 0;

    /* getSize - accessor method for the number of items in the PQ
     * @return int, number of elements in the PQ
     */
    virtual int  getSize() = 0;       
    
    /* isEmpty - returns true if there are no elements in the PQ
     * @return bool, whether there are no elements in the PQ
     */
    virtual bool isEmpty() = 0;      
};

#endif 
