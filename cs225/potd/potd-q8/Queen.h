#ifndef _QUEEN_H
#define _QUEEN_H
#include <string>
#include <iostream>
#include "Piece.h"

class Queen : public Piece {
public:
  string getType();
};

#endif
