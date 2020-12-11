// Pet.cpp
#include <iostream>
#include <string>
#include "Animal.h"
#include "Pet.h"

using namespace std;

void Pet::setFood(string new_food) {
    food_ = new_food;
}

string Pet::getFood() {
    return food_;
}

void Pet::setName(string new_name) {
    name_ = new_name;
}

string Pet::getName() {
    return name_;
}
void Pet::setType(string new_type) {
    type_ = new_type;
}

string Pet::getType() {
    return type_;
}
void Pet::setOwnerName(string new_ownername) {
    owner_name_ = new_ownername;
}

string Pet::getOwnerName() {
    return owner_name_;
}

string Pet::printname() {
    return "My name is" + name_;
};

Pet::Pet() : type_("cat"), food_("fish"), name_("Fluffy"), owner_name_("Cinda") { }

Pet::Pet(string type, string food, string name_, string owner_name_) : type_(type), food_(food), name_(name_), owner_name_(owner_name_) { }
