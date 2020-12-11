// Your code here
#include "Food.h"
#include <cstring>
#include <string>

using namespace std;
Food::Food(){

}

int Food::set_quantity(int new_num){
  quantity_=new_num;
  return quantity_;
}
int Food::get_quantity(){
  return quantity_;
}
string Food::set_name(string new_name){
  name_=new_name;
  return name_;
}
string Food::get_name(){
  return name_;
}
