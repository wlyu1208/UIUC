#include <iostream>
#include <string>
using namespace std;

#include "Square.h"

Square::Square() {
    name = "mysquare";
    lengthptr = new double;
    *lengthptr = 2.0;
}

void Square::setName(string newName) {
  name = newName;
}

void Square::setLength(double newVal) {
  *lengthptr = newVal;
}

string Square::getName() const {
  return name;
}

double Square::getLength() const {
  return *lengthptr;
}

Square::Square(const Square & other) {
    name = other.getName();
    lengthptr = new double;
    *lengthptr = other.getLength();
}

Square::~Square() {
    delete lengthptr;
}

Square & Square::operator=(const Square & other){
  if(&other==this){
    return *this;
  }
  name = other.getName();
  *lengthptr=other.getLength();
  return *this;
}
Square Square::operator+(const Square & other){
  Square result = *this;

  string result_name = name + other.name;
  double result_length =  *lengthptr + *other.lengthptr;

  result.setName(result_name);
  result.setLength(result_length);

  return result;
}
