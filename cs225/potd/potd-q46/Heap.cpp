#include "Heap.h"
#include <iostream>
using namespace std;
void Heap::_percolateDown(int hole) {
    // your code here
    int a;
    while((unsigned)hole*2 <= _data.size()-1){
      if(_data[hole*2]>_data[hole*2+1]){
        a=hole*2;
      }
      else{
        a=hole*2+1;
      }
      if(_data[hole]<_data[a]){
        int max=a;
        swap(_data[hole], _data[a]);
        hole=max;
      }
      else{
        break;
      }
    }
}

int Heap::size() const {
    return _data.size();
}

void Heap::enQueue(const int &x){
    _data.push_back(x);
    int hole = _data.size() - 1;
    for(; hole > 1 && x > _data[hole/2]; hole /= 2){
        _data[hole] = _data[hole/2];
    }
    _data[hole] = x;
}

int Heap::deQueue(){
    int minItem = _data[1];
    _data[1] = _data[_data.size() - 1];
    _data.pop_back();
    _percolateDown(1);
    return minItem;
}

void Heap::printQueue(){
    std::cout << "Current Priority Queue is: ";
    for(auto i = _data.begin() + 1; i != _data.end(); ++i){
        std::cout << *i << " ";
    }
    std::cout << std::endl;
}

std::vector<int> & Heap::getData() {
    return _data;
}
