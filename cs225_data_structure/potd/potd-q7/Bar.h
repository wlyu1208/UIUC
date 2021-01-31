// your code here
#ifndef BAR_H
#define BAR_H

#include "Foo.h"
#include <string>

using namespace std;

namespace potd{
  class Bar{
  private:
    Foo* f_;
  public:
    Bar(string x);
    Bar(const Bar &);
    Bar & operator=(const Bar &);
    ~Bar();
    string get_name();

  };
}
#endif
