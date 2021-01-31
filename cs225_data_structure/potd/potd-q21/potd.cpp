#include "potd.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;
// Your code here!

double sum(vector<double>::iterator start, vector<double>::iterator end){
  double sum2=0.0;
  for( ; start !=end; ++start){
  sum2 += *start;
}
  return sum2;
}

vector<double>::iterator max_iter(vector<double>::iterator start, vector<double>::iterator end){
  vector<double>::iterator largest = start;
  for(; start!=end; ++start){
    if(*largest<*start){
      largest=start;
    }
  }
  return largest;
}

void sort_vector(vector<double>::iterator start, vector<double>::iterator end){
  vector<double>::iterator max_num;
  if(start==end){
    return;
  }
  max_num=max_iter(start, end);
  std::swap(*start, *max_num);
  start++;
  sort_vector(start,end);
  
}
