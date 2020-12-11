#include <vector>
#include <string>
#include "Hash.h"
#include <iostream>
#include <map>

using namespace std;

int hashFunction(string s, int M) {
   // Your Code Here
   //hash function to sum up the ASCII characters of the letters of the string
   int result=0;
   for(unsigned i=0; i<s.length(); i++){
     result=result+s[i];
   }
   return result%M;
 }

int countCollisions (int M, vector<string> inputs) {
	int collisions = 0;
	// Your Code Here
  vector<int> history;
  map<int, int> mapping;

  for(unsigned i=0; i<inputs.size(); i++){
    history.push_back(hashFunction(inputs[i], M));
  }

  for(unsigned i=0; i<history.size(); i++){
    mapping[history[i]]++;
  }

  for(auto &it: mapping){
    if(it.second!=1){
      collisions=collisions+(it.second-1);
    }
  }
	return collisions;
}
