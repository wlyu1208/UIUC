#ifndef _ANIMAL_H
#define _ANIMAL_H
#include <string>

using namespace std;

class Animal{
private:
  string type_;
  string food_;
public:
  Animal();
  Animal(string ANIMAL, string FOOD);
  void setType(string new_type);
  void setFood(string new_food);
  string getType();
  string getFood();
  string print();
};
#endif
