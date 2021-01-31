// your code here
#include <iostream>
#include "Food.h"
#include "q5.h"
#include <cstring>
#include <string>

using namespace std;

int main(){
  Food* Food2 = new Food();
  int w = Food2->get_quantity();
  string name2 = Food2->get_name();
  cout<<"You have "<<(w)<<" "<<name2<<"."<<endl;
  int* f;
  int v = void increase_quantity(f);
  cout<<"You have "<<(v)<<" "<<name2<<"."<<endl;
  return 0;
}
