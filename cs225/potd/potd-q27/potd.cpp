// Your code here
#include <iostream>
#include "potd.h"
#include <string>

string getFortune(const string &s){
  int temp = s.length() % 5;
  if(temp==1){
    return "Good";
  }
  if(temp==2){
    return "Bad";
  }
  if(temp==3){
    return "Soso";
  }
  if(temp==4){
    return "Not Bad";
  }
    return "Excellent";
}
