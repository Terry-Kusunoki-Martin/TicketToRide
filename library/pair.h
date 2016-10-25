#ifndef PAIR_H_
#define PAIR_H_

/**
 * A Pair is an container class for two pieces of data, which it
 * stores publicly.  
 */
template <typename F, typename S>
class Pair {
  public:
    F first;   // The first item in the pair.
    S second;  // The second item in the pair.

    Pair() {};
    Pair(F f, S s) {first = f; second = s;};
   
         

};

template <typename F, typename S>
inline bool operator==(Pair<F,S> p1,Pair<F,S> p2){
  if(p1.first == p2.first && p1.second == p2.second){
    return true;
  }
  return false;
}
#endif
