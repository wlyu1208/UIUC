// Animal.cpp
#include <iostream>
#include "Animal.h"
#include <string>

using namespace std;

Animal::Animal(){
  this->type_="cat";
  this->food_="fish";
}

Animal::Animal(string ANIMAL, string FOOD){
  this->type_=ANIMAL;
  this->food_=FOOD;
  this->setType(ANIMAL);
  this->setFood(FOOD);
}

void Animal::setType(string new_type){
  this->type_=new_type;
}
void Animal::setFood(string new_food){
  this->food_=new_food;
}

string Animal::getType(){
  return type_;
}
string Animal::getFood(){
  return food_;
}
string Animal::print(){
  return "I am a " + getType() + ".";
}
