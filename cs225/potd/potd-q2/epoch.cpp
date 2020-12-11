#include "epoch.h"
/* Your code here! */

int hours(time_t sec_since_epoch){
  sec_since_epoch=sec_since_epoch/3600;
  return sec_since_epoch;
}

int days(time_t sec_since_epoch){
  sec_since_epoch=sec_since_epoch/24/3600;
  return sec_since_epoch;
}

int years(time_t sec_since_epoch){
  sec_since_epoch=sec_since_epoch/365/24/3600;
  return sec_since_epoch;
}
