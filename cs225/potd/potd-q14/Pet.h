#ifndef _PET_H
#define _PET_H
#include <string>
#include "Animal.h"

class Pet: public Animal{
private:
  string name_;
  string owner_name_;
  string food_;
  string type_;
public:
  Pet();
  Pet(string type_, string food_, string name_, string owner_name_);
  void setFood(string new_food);
  string getFood();
  void setName(string new_name);
  string getName();
  void setType(string new_type);
  string getType();
  void setOwnerName(string new_ownername);
  string getOwnerName();
  string printname();
};
#endif
