// your code here
#include "Foo.h"
#include "Bar.h"
#include <string>

using namespace std;
using namespace potd;

Bar::Bar(string name){
  f_ = new Foo(name);
}

Bar::Bar(const Bar &that){
  f_ = new Foo(that.f_->get_name());
}

Bar & Bar::operator=(const potd::Bar &that){
  if(this == &that){
  return *this;
  }
  f_->~Foo();
  f_ = new Foo(*that.f_);
  return *this;
}

Bar::~Bar(){
  f_->~Foo();
}

string Bar::get_name(){
  return f_->get_name();
}
