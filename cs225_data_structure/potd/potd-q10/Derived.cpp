#include <iostream>
#include "Base.h"
#include "Derived.h"
#include <string>

using namespace std;

string Derived::foo(){
  return "I will not eat them.";
}

string Derived::bar(){
  return "And Ham";
}
