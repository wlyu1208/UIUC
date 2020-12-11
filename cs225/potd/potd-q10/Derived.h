#ifndef DERIVED_H
#define DERIVED_H
#include <string>
#include "Base.h"

using namespace std;

class Derived : public Base{
public:
  string foo();
  virtual string bar();
};

#endif
