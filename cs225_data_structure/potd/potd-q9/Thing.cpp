// Your code here!
#include <iostream>
#include "Thing.h"
#include <string>

using namespace std;

namespace potd{
  Thing::Thing(int size){
    props_ct_=0;
    props_max_=size;
    properties_= new string[props_max_];
    values_= new string[props_max_];
  }

  Thing::Thing(const Thing &copy_const){
    _copy(copy_const);
  }

  Thing& Thing::operator=(const Thing &op){
    if(this==&op){
      return *this;
    }
    _destroy();
    _copy(op);
    return *this;
  }

  Thing::~Thing(){
    _destroy();
  }

  int Thing::set_property(string name, string value){
    for(int i=0; i<props_ct_;i++){
      if(properties_[i]==name){
        values_[i]=value;
        return i;
      }
    }
    if(props_max_==props_ct_){
      return -1;
    }
    properties_[props_ct_]=name;
    values_[props_ct_]=value;
    return props_ct_++;

  }

  string Thing::get_property(string name){
    for(int i=0; i<props_ct_; i++){
      if(properties_[i]==name){
        return values_[i];
      }
    }
    return "";
  }

  void Thing::_copy(const Thing &copy_const){
    this->props_max_=copy_const.props_max_;
    this->props_ct_=copy_const.props_ct_;
    this->properties_=copy_const.properties_;
    this->values_=copy_const.values_;

    this->properties_= new string[props_max_];
    this->values_= new string[props_max_];

    for(int i=0; i<props_max_; i++){
      values_[i]=copy_const.values_[i];
      properties_[i]=copy_const.properties_[i];
    }
  }

  void Thing::_destroy(){
    delete[] properties_;
    delete[] values_;
    props_ct_=0;
    properties_=NULL;
    values_=NULL;
  }
}
