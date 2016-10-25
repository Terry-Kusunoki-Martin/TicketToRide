#ifndef LINKEDBST_H_
#define LINKEDBST_H_

#include "BST.h"
#include "pair.h"
#include "queue.h"

// Forward declaration of the BSTNode class, so we can use it in
// LinkedBST before we've defined it.  This is like declaring a function
// before defining it
template <typename K, typename V> class BSTNode;

/**
 *  A LinkedBST is a templated binary search tree, implementing the
 *  BST interface using linked node structures.
 *  Most of the externally-accessible LinkedBST functions
 *  simply call a private recursive methods, that performs the
 *  required function on a given sub-tree.
 *  (see BST.h for documentation on inherited functions)
 */
template <typename K, typename V>
class LinkedBST : public BST<K,V> {
  private:
    int size;            // Current number of items in the tree.
    BSTNode<K,V>* root;  // Pointer to the root node (possibly NULL).

  public:
    LinkedBST();
    ~LinkedBST();
    
    /* All public functions defined in BST.h*/
    /* These methods are defined in linkedBST-inl.h*/
    
    /* sizing operations */
    int getSize();
    bool isEmpty();
    int  getHeight();

    /* Key operations */
    K    getMax();
    K    getMin();
    
    /* dictionary operations */
    void insert  (K key, V value);  
    void update  (K key, V value);  
    bool contains(K key);           
    void remove  (K key);           
    V    find    (K key);           
  
    /* traversal operations */
    Queue< Pair<K,V> >* getPreOrder();
    Queue< Pair<K,V> >* getInOrder();
    Queue< Pair<K,V> >* getPostOrder();
    Queue< Pair<K,V> >* getLevelOrder();

  private:                          
    /* Private recursive internal methods that
     * correspond to the public methods defined above.
     * These methods are defined in linkedBST-private-inl.h
     */

    /* insertInSubtree - Recursive function that inserts a new node into 
     *                   a sub-tree pointed to by current
     * @param current : a pointer to the root of the sub-tree
     * @param key : the key for the new node being inserted
     * @param value : the value for the new node being inserted
     * @error runtime_error if the key already exists
     * @return BSTNode<K,V>*: the root of the sub-tree.  This is for the parent
     *        node to set it's left or right child.  On an actual insert,
     *        the pointer will be to the newly allocated node.  Otherwise,
     *        the structure hasn't changed and we return a pointer to current
     */    
    BSTNode<K,V>* insertInSubtree   (BSTNode<K,V>* current, K key, V value);

    /* updateInSubtree - Recursive function that updates a key-value pair
     *                   in the tree
     * @param current : pointer to root node of the sub-tree
     * @param key : the key being searched for
     * @param value : the new value to associate with the given key
     * @error runtime_error if key not found
     */
    void updateInSubtree     (BSTNode<K,V>* current, K key, V value); 
    
    /* removeFromSubtree - Recursive function that searches for and removes
     *                   the element associated with the given key in the
     *                   sub-tree pointed to by current
     * @param current : a pointer to the root of the sub-tree
     * @param key : the key for the node to be removed
     * @error runtime_error if the key does not exist
     * @return BSTNode<K,V>*: the root of the sub-tree.  This is for the parent
     *        node to set it's left or right child
     */  
    BSTNode<K,V>* removeFromSubtree (BSTNode<K,V>* current, K key);

    /* containsInSubtree - Recursive function that checks if the sub-tree
     *                 pointed by current contains the given key
     * @param current : pointer to root node of the sub-tree
     * @param key : the key being searched for
     * @return bool : true if the key was found
     */
    bool          containsInSubtree (BSTNode<K,V>* current, K key); 

    /* findInSubtree - Recursive function that returns the value associated
     *                 with the given key in the sub-tree
     *                 pointed by current 
     *@param current : pointer to root node of the sub-tree
     *@param key : the key being searched for
     *@error runtime_error if the key is not in the sub-tree
     *@return V : the value associated with the search key
     */
    V             findInSubtree     (BSTNode<K,V>* current, K key); 

    /* getMaxInSubtree - Recursive function that retrieves the maximal key
     *                   in the sub-tree pointed to by current
     *
     * @param  current: pointer to root node of the sub-tree
     * @return K : the maximal key in the subtree
     */
    K             getMaxInSubtree   (BSTNode<K,V>* current);

    /* getMinInSubtree - Recursive function that retrieves the minimal key
     *                   in the sub-tree pointed to by current
     *
     * @param  current: pointer to root node of the sub-tree
     * @return K : the minimal key in the subtree
     */
    K             getMinInSubtree   (BSTNode<K,V>* current);

    /* getHeightOfSubtree -  Recursive function that calculates the height
     *                       of the sub-tree pointed to be current
     *
     * @param current: pointer to root node of the sub-tree
     * @return int : the maximal depth of a node in the subtree; -1 if
     *               current is NULL
     */
    int           getHeightOfSubtree(BSTNode<K,V>* current);

    /* build{Pre,In,Post} - Recursive helper functions for building 
     *                      iterators for the data structure.
     *                      Each enqueues all key-value pairs for the 
     *                      sub-tree pointed to by current
     *
     * @param current : pointer to root node of the sub-tree
     * @param it : a pointer to the Queue to fill with the key-value
     *                             pairs based on the traversal order
     */
    void buildPreOrder (BSTNode<K,V>* current, Queue< Pair<K,V> >* it);
    void buildInOrder  (BSTNode<K,V>* current, Queue< Pair<K,V> >* it);
    void buildPostOrder(BSTNode<K,V>* current, Queue< Pair<K,V> >* it);

    /* traverseAndDelete - Recursive helper function for the destructor
     *                     Performs a post-order traversal of the sub-tree
     *                     freeing memory for all nodes in the sub-tree
     *                     pointed to by current
     * @param current -  : pointer to root node of the sub-tree to be deleted
     */
    void traverseAndDelete (BSTNode<K,V>* current);
};


/*****************************************************************************/


/**
 * The BSTNode is a templated class that stores data for each node
 * in the LinkedBST.
 */
template <typename K, typename V>
class BSTNode {
  private:
    K key;                      // The key stored in this node.
    V value;                    // The value stored in this node.
    BSTNode<K,V>* left;         // Pointer to this node's left subtree.
    BSTNode<K,V>* right;        // Pointer to this node's right subtree.

    /* default constructor */
    BSTNode();    

    /* preferred constructor to initialize key and value to given
     * parameters; left and right are set to NULL
     */
    BSTNode(K k, V v);
 
  /* indicates that LinkedBST is a friend class so that it can directly
   * access private aspects of this class.
   * friend is a C++ keyword that allows another function or class to 
   * access all features.  Note that without this, we cannot actually
   * create a BSTNode since the constructors are private
   */
  friend class LinkedBST<K,V>;
};

//all the public methods are defined here as well as the BSTNode class
#include "linkedBST-inl.h"

//all the private methods are defined here
#include "linkedBST-private-inl.h"

#endif  // LINKEDBST_H_
