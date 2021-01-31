#include <vector>
#include "Hash.h"

void doubleHashInput(std::vector<int> &v, int elem){
	//your code here
  unsigned index=firstHash(elem, v.size());
  if(v[index]==-1){
    v[index]=elem;
  }
  else{
    unsigned size=secondHash(elem);
    unsigned copy=index;
    unsigned i=0;
    while(v[index]!=-1){
      index=copy+i*size;
      while(index>=v.size()){
        index=index-v.size();
      }
      i++;
    }
    v[index]=elem;
  }
}

//make a hash function called firstHash
//make a second function called secondHash
int firstHash(int elem, int length){
  return (4*elem)%length;
}

int secondHash(int elem){
  return 3-(elem%3);
}
