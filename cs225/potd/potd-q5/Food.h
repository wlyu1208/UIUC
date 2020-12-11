// Your code here
#ifndef _FOOD_H
#define _FOOD_H
#include <cstring>
#include <string>

using namespace std;
class Food {
  private:
    string name_;
    int quantity_=5;

  public:
    Food();
    string get_name();
    string set_name(string new_name);
    int get_quantity();
    int set_quantity(int new_num);
};

#endif
