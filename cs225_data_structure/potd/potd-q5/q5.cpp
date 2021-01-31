// your code here
#include <iostream>
#include "Food.h"
#include "q5.h"
#include <string>

using namespace std;

void increase_quantity(Food* f){
  int new_quantity = f->get_quantity()+1;
  f->set_quantity(new_quantity);
}
