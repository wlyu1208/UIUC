#include "maze.h"
#include <iostream>

using namespace std;

SquareMaze::SquareMaze(){

}

void SquareMaze::makeMaze(int width, int height) {
  maze_w=width;
  maze_h=height; //get width and height of maze
  s.addelements(maze_w*maze_h); //set element of area

  for(int i=0; i<maze_w; i++){
    for(int j=0; j<maze_h; j++){
      rightwall.push_back(true);
      downwall.push_back(true); //on area, insert true for all
    }
  }

  map<int, bool> b_wall, r_wall; //initialize wall for checking
  vector<pair<int, int>> d_wall;

  for(int i=0; i<maze_w; i++){
    for(int j=0; j<maze_h; j++){
      int idx=i*maze_h+j; //get index to insert
      if(j+1<maze_h){
        d_wall.push_back(pair<int, int>(i, j));
        b_wall[idx]=true;
      } //within width and height, insert place of wall to delete for right and bottom
      if(i+1<maze_w){
        d_wall.push_back(pair<int, int>(i, j));
        r_wall[idx]=true;
      }
    }
  }

  unsigned long seed=12345087637;
  srand(time(0)+seed);

  random_shuffle(d_wall.begin(), d_wall.end()); //randomly mix the vector

  queue<pair<int, int>> temp;

  for(unsigned long i=0; i<d_wall.size(); i++){
    temp.push(d_wall[i]); //push all data into the queue
  }

  while(!temp.empty()){
    pair<int, int> data=temp.front();
    temp.pop(); //get first data
    int num=rand()%2; //choose 0 or 1
    int place=data.first*maze_h+data.second; //get index
    int next=0;
    int path;//get path and neighbors

    if(r_wall[place]==true && b_wall[place]==true){ //if right and bottom wall are true
      if(num==0){
        path=1;
        next=data.first*maze_h+(data.second+1);
      } //path become 1  and increase right
      else{
        path=0;
        next=(data.first+1)*maze_h+data.second;
      } //path become 0 and increase bottom
    }
    if(r_wall[place]==false && b_wall[place]==true){
      path=1;
      next=data.first*maze_h+(data.second+1);
    } //only increase right
    if(r_wall[place]==true && b_wall[place]==false){
      path=0;
      next=(data.first+1)*maze_h+data.second;
    } //only increase bottom

    if(s.find(place)!=s.find(next)){
      int idx=data.second*maze_w+data.first; //set index
      if(path==1){ //depend on path, down or right become false
        downwall[idx]=false;
      }
      if(path==0){
        rightwall[idx]=false;
      }
      s.setunion(place, next); //unite index and neighbors
    }
  }

}

bool SquareMaze::canTravel(int x, int y, int dir) const {
  int idx=y*maze_w+x; //get index
  if(dir==0 && x+1<maze_w && (!rightwall[idx])){
    return true;
  }//if dir is 0 and x within width and right is false

  if(dir==1 && y+1<maze_h && (!downwall[idx])){
      return true;
  }//if dir is 1 and y is wihtin height and bottom is false

  idx=y*maze_w+(x-1); //set new index
  if(dir==2 && x-1>=0 && (!rightwall[idx])){
      return true;
  }//if dir is 2 and x is greater than 0 when right wall is false

  idx=(y-1)*maze_w+x; //set new index
  if(dir==3 && y-1>=0 && (!downwall[idx])){
      return true;
  }//if dir is 3 and y is greater than 0 when bottom wall is false
  return false; //else return false
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
  int idx=y*maze_w+x;
  if(x<maze_w && y<maze_h){
    if(dir==1){
      downwall[idx]=exists;
    }
    else{
      rightwall[idx]=exists;
    } //depend on dir, change depended wall on index
  } //within range of maze
}

vector<int> SquareMaze::solveMaze() {

  vector<bool> answer; //initialize solution

  for(int i=0; i<maze_h*maze_w; i++){
    answer.push_back(false);
  } //for size of maze, put false

  queue<pair<int, int>> temp;
  temp.push(pair<int, int>(0, 0)); //use quere to temproary store answer

  map<int, int> path;
  map<pair<int, int>, pair<int, int>> prt;
  pair<int, int> current; //initialize necessary variables

  while(!temp.empty()){ //until quue is empty
    current=temp.front();
    temp.pop(); //get first value of queue

    int x=current.first;
    int y=current.second; //get first and second data

    int r=y*maze_w+(x+1);
    int d=(y+1)*maze_w+x;
    int l=y*maze_w+(x-1);
    int u=(y-1)*maze_w+x; //get right left up down variables

    pair<int, int> pt (x,y);

    if(canTravel(x, y, 3) && !answer[u]){
      prt[pair<int, int>(x, y-1)]=pt;
      temp.push(pair<int, int>(x, y-1));
      answer[u]=true;
      path[u]=3;
    }
    if(canTravel(x, y, 2) && !answer[l]){
      prt[pair<int, int>(x-1, y)]=pt;
      temp.push(pair<int, int>(x-1, y));
      answer[l]=true;
      path[l]=2;
    }
    if(canTravel(x, y, 1) && !answer[d]){
      prt[pair<int, int>(x, y+1)]=pt;
      temp.push(pair<int, int>(x, y+1));
      answer[d]=true;
      path[d]=1;
    }
    if(canTravel(x, y, 0) && !answer[r]){ //depend on travel, make a mark
      prt[pair<int, int>(x+1, y)]=pt;
      temp.push(pair<int, int>(x+1, y));
      answer[r]=true;
      path[r]=0;
    }
  }

  vector<pair<int, int>> goal;

  for(int i=0; i<maze_w; i++){
    goal.push_back(pair<int, int>(i, maze_h-1));
  } //get size of maze height for each width

  vector<int> solution; //initialize return value

  for(auto it:goal){
    int x=it.first;
    int y=it.second;
    vector<int> way;
    int c, d; //initialize necessary variables

    while(true){
      int c=prt[pair<int, int>(x, y)].first;
      int d=prt[pair<int, int>(x, y)].second;
      int a=x-c;
      int b=y-d;

      if(a==1){
        way.push_back(0);
      }
      if(a==-1){
        way.push_back(2);
      }

      if(b==1){
        way.push_back(1);
      }
      if(b==-1){
        way.push_back(3);
      } //depend on x value and y value, push path to go on way vector

      x=c;
      y=d;

      if(c<1 && d<1){
        break;
      } //if parent are 0 then stop
    }
    if(solution.size()<way.size()){
      solution=way;
    } //if path is greater than solution, make solution to way
  }

  reverse(solution.begin(),solution.end());
  return solution; //reverse solution and return
}

PNG* SquareMaze::drawMaze() const{
  PNG* pic=new PNG(10*maze_w+1, 10*maze_h+1);
  vector<unsigned long> start; //get size of maze pic and vector to use

  for(unsigned long i=1; i<10; i++){
    start.push_back(i);
  } //create vector size of 9

  for(int i=0; i<10*maze_h+1; i++){
    (pic->getPixel(0,i)).h=0;
    (pic->getPixel(0,i)).s=0;
    (pic->getPixel(0,i)).l=0;
  } //for height, make h s l to 0

  for(int i=0; i<10*maze_w+1; i++){
    if(find(start.begin(), start.end(), i)==start.end()){
      (pic->getPixel(i,0)).h=0;
      (pic->getPixel(i,0)).s=0;
      (pic->getPixel(i,0)).l=0;
    }
  } //for width, if end is same make h s l to 0

  for(int i=0; i<maze_w; i++){
    for(int j=0; j<maze_h; j++){ //for within maze size
      int idx=j*maze_w+i; //get index

      if(downwall[idx]==true){ //if down is true make change
        for(int e=0; e<11; e++){
          int x=(i*10)+e;
          int y=(j+1)*10;
          pic->getPixel(x, y).h=0;
          pic->getPixel(x, y).s=0;
          pic->getPixel(x, y).l=0;
        }
      }

      if(rightwall[idx]==true){ //if right is true make change
        for(int e=0; e<11; e++){
          int x=(i+1)*10;
          int y=(j*10)+e;
          pic->getPixel(x, y).h=0;
          pic->getPixel(x, y).s=0;
          pic->getPixel(x, y).l=0;
        }
      }
    }
  }
return pic;
}

PNG* SquareMaze::drawMazeWithSolution(){
  if(maze_h*maze_w==0){
    return new PNG();
  } //if size is 0 than return empty pic

  PNG* pic=drawMaze();
  vector<int> solution=solveMaze(); //get maze with pic and get solution on vector

  int a=5;
  int b=5; //set first co-ord to 5,5

  for(unsigned long i=0; i<solution.size(); i++){ //for size of maze

    if(solution[i]==3){
      for(int x=0; x<=10; x++){
        pic->getPixel(a, b).h=0;
        pic->getPixel(a, b).s=1;
        pic->getPixel(a, b).l=0.5;
        b=b-1;
      }
      b=b+1;
    } //if path is 3 than change pixel and decrease height for 10

    if(solution[i]==2){
      for(int x=0; x<=10; x++){
        (pic->getPixel(a, b)).h=0;
        (pic->getPixel(a, b)).s=1;
        (pic->getPixel(a, b)).l=0.5;
        a=a-1;
      }
      a=a+1;
    } //if path is 2 then change pixel and decrease width for 10

    if(solution[i]==1){
      for(int x=0; x<=10; x++){
        (pic->getPixel(a, b)).h=0;
        (pic->getPixel(a, b)).s=1;
        (pic->getPixel(a, b)).l=0.5;
        b=b+1;
      }
      b=b-1;
    } //if path is 1 then change pixel and increase height by 10

    if(solution[i]==0){
      for(int x=0; x<=10; x++){
        (pic->getPixel(a, b)).h=0;
        (pic->getPixel(a, b)).s=1;
        (pic->getPixel(a, b)).l=0.5;
        a=a+1;
      }
      a=a-1;
    } //if path is 0 then change pixel and increase width by 10

  }

  int j=a/10;
  int k=maze_h*10;

  for(int y=1; y<10; y++){
    pic->getPixel(j*10+y, k).h=0;
    pic->getPixel(j*10+y, k).s=0;
    pic->getPixel(j*10+y, k).l=1;
  }
  return pic;
}
