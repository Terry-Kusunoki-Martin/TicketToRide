/*
 * Default constructor
 */
template <typename V, typename E, typename W>
Edge<V,E,W>::Edge() {
  weight = 1;
}

/*
 * Initializes the src, dest, label, and weight of the edge
 */
template <typename V, typename E, typename W>
Edge<V,E,W>::Edge(V* src, V* dest, E label, W weight) {
  this->src = src;
  this->dest = dest;
  this->label = label;
  this->weight = weight;
}

/*
 * Getter methods
 */
template <typename V, typename E, typename W>
E Edge<V,E,W>::getLabel() {
  return label;
}

template <typename V, typename E, typename W>
V Edge<V,E,W>::getSrc() {
  return *src;
}

template <typename V, typename E, typename W>
V Edge<V,E,W>::getDest() {
  return *dest;
}

template <typename V, typename E, typename W>
W Edge<V,E,W>::getWeight() {
  return weight;
}
