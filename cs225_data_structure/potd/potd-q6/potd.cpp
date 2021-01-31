// your code here!
#include <iostream>
#include "potd.h"
#include <vector>
#include <cmath>

using namespace std;
using namespace potd;

int* potd::raise(int *arr){
int i=0;
vector<int> new_vector;

while(arr[i]!=-1 && arr[i+1]!=-1){
  int data = pow(arr[i], arr[i+1]);
  new_vector.push_back(data);
  i++;
  }
  new_vector.push_back(arr[i]);
  new_vector.push_back(-1);

  int size = i+1;
  int* array = new int[size];

  for (unsigned long i=0; i<new_vector.size();i++){
    array[i]=new_vector[i];
  }
  return array;
}
