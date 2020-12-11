//
// Write a templated function `more` which takes in two variables of the same
// type and returns whichever variable is greater than (`>`) the other.
//
#ifndef POTD_H
#define POTD_H

template<typename T> const T & more(const T &one, const T &two) {
    // your code here
    int first = one.getQuantity();
    int second = two.getQuantity();
    if(first>second){
      return one;
    }
    else{
      return two;
    }
}
#endif
