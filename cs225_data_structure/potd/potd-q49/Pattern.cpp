
#include "Pattern.h"
#include <string>
#include <iostream>
#include <vector>
#include <cctype>

using namespace std;

bool wordPattern(string pattern, string str) {

	  vector<string> v;
    string tem="";

    for(unsigned i = 0; i<=str.size(); i++){
      if(isalpha(str[i])){tem =str[i] + tem;}

      else{
        v.push_back(tem);
        tem="";
      }
    }
    char stringchar = 'a';
  	char t[v.size()];
      t[0] = (stringchar);
    bool checkf = false;
    for(unsigned j = 1; j<v.size(); j++){
      checkf = false;
      for(unsigned k = 0; k<j; k++){
        if(v[j]==v[k]){
          checkf = true;
          t[j]=t[k];
          break;
        }
      }
      if(!checkf){
        stringchar++;
        t[j]=(stringchar);
      }
    }
    checkf = false;
    vector<char> p;
    for(unsigned i = 0; i<=pattern.size(); i++){
      if(isalpha(pattern[i])){p.push_back(pattern[i]);}
    }
      for(unsigned g = 0; g<p.size();g++){
      if(p[g]==t[g]){continue;}
      else{return false;}
    }
    return true;
}
