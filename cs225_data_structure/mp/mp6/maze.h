/* Your code here! */
#ifndef MAZE_H
#define MAZE_H
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <queue>
#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "dsets.h"

using namespace std;
using namespace cs225;

class SquareMaze {
public:
  SquareMaze();
  void makeMaze(int width, int height);
  bool canTravel(int x, int y, int dir) const;
  void setWall(int x, int y, int dir, bool exists);
  vector<int> solveMaze();
  PNG* drawMaze() const;
  PNG* drawMazeWithSolution();
private:
  int maze_w;
  int maze_h;
  DisjointSets s;
  vector<bool> rightwall;
  vector<bool> downwall;
};

#endif
