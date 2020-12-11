/**
 * @file NimLearner.cpp
 * CS 225: Data Structures
 */

#include "NimLearner.h"
#include <ctime>

using namespace std;
/**
 * Constructor to create a game of Nim with `startingTokens` starting tokens.
 *
 * This function creates a graph, `g_` representing all of the states of a
 * game of Nim with vertex labels "p#-X", where:
 * - # is the current player's turn; p1 for Player 1, p2 for Player2
 * - X is the tokens remaining at the start of a player's turn
 *
 * For example:
 *   "p1-4" is Player 1's turn with four (4) tokens remaining
 *   "p2-8" is Player 2's turn with eight (8) tokens remaining
 *
 * All legal moves between states are created as edges with initial weights
 * of 0.
 *
 * @param startingTokens The number of starting tokens in the game of Nim.
 */
NimLearner::NimLearner(unsigned startingTokens) : g_(true, true) {
    /* Your code goes here! */

    for(unsigned int i=0; i<=startingTokens; i++){
      string x=to_string(i);
      g_.insertVertex("p1-"+x);
      g_.insertVertex("p2-"+x);
    }

    for(unsigned int i=startingTokens; i>0; i--){
      if(i!=1){
        string x=to_string(i);
        string y=to_string(i-1);
        string z=to_string(i-2);

        g_.insertEdge("p1-"+x, "p2-"+y);
        g_.setEdgeWeight("p1-"+x, "p2-"+y, 0);

        g_.insertEdge("p1-"+x, "p2-"+z);
        g_.setEdgeWeight("p1-"+x, "p2-"+z, 0);

        g_.insertEdge("p2-"+x, "p1-"+y);
        g_.setEdgeWeight("p2-"+x, "p1-"+y, 0);

        g_.insertEdge("p2-"+x, "p1-"+z);
        g_.setEdgeWeight("p2-"+x, "p1-"+z, 0);
      }
      else{
        string x=to_string(i);
        string y=to_string(i-1);

        g_.insertEdge("p1-"+x, "p2-"+y);
        g_.setEdgeWeight("p1-"+x, "p2-"+y, 0);

        g_.insertEdge("p2-"+x, "p1-"+y);
        g_.setEdgeWeight("p2-"+x, "p1-"+y, 0);

      }
    }

    count=startingTokens;
    startingVertex_="p1-"+to_string(count);
}

/**
 * Plays a random game of Nim, returning the path through the state graph
 * as a vector of `Edge` classes.  The `origin` of the first `Edge` must be
 * the vertex with the label "p1-#", where # is the number of starting
 * tokens.  (For example, in a 10 token game, result[0].origin must be the
 * vertex "p1-10".)
 *
 * @returns A random path through the state space graph.
 */
std::vector<Edge> NimLearner::playRandomGame() const {
  vector<Edge> path;
 /* Your code goes here! */
 Vertex start=startingVertex_;
 string player;
 string player1="p1-";
 string player2="p2-";
 int token=count;
 int turn;
 player=player2;
 while(token>0){
   if(token!=1){
     turn=rand()%2+1;
   }
   else{
     turn=1;
   }
   token=token-turn;
   path.push_back(g_.getEdge(start, player+to_string(token)));
   start=player+to_string(token);
   if(player==player2){
     player=player1;
   }
   else{
     player=player2;
   }
 }
  return path;
}

/*
 * Updates the edge weights on the graph based on a path through the state
 * tree.
 *
 * If the `path` has Player 1 winning (eg: the last vertex in the path goes
 * to Player 2 with no tokens remaining, or "p2-0", meaning that Player 1
 * took the last token), then all choices made by Player 1 (edges where
 * Player 1 is the source vertex) are rewarded by increasing the edge weight
 * by 1 and all choices made by Player 2 are punished by changing the edge
 * weight by -1.
 *
 * Likewise, if the `path` has Player 2 winning, Player 2 choices are
 * rewarded and Player 1 choices are punished.
 *
 * @param path A path through the a game of Nim to learn.
 */
void NimLearner::updateEdgeWeights(const std::vector<Edge> & path) {
 /* Your code goes here! */
 if(path[path.size()-1].dest=="p1-0"){
   for(unsigned long i=0; i<path.size(); i++){
     int weight=g_.getEdgeWeight(path[i].source, path[i].dest);
     Vertex a=path[i].source;
     Vertex b=path[i].dest;

     if(i%2==0){
       g_.setEdgeWeight(a, b, weight-1);
     }
     else{
       g_.setEdgeWeight(a, b, weight+1);
     }

   }
   return;
 }
 else{
   for(unsigned long i=0; i<path.size(); i++){
     int weight=g_.getEdgeWeight(path[i].source, path[i].dest);
     Vertex a=path[i].source;
     Vertex b=path[i].dest;

     if(i%2==0){
       g_.setEdgeWeight(a, b, weight+1);
     }
     else{
       g_.setEdgeWeight(a, b, weight-1);
     }

   }
 }
}

/**
 * Label the edges as "WIN" or "LOSE" based on a threshold.
 */
void NimLearner::labelEdgesFromThreshold(int threshold) {
  for (const Vertex & v : g_.getVertices()) {
    for (const Vertex & w : g_.getAdjacent(v)) {
      int weight = g_.getEdgeWeight(v, w);

      // Label all edges with positve weights as "WINPATH"
      if (weight > threshold)           { g_.setEdgeLabel(v, w, "WIN"); }
      else if (weight < -1 * threshold) { g_.setEdgeLabel(v, w, "LOSE"); }
    }
  }
}

/**
 * Returns a constant reference to the state space graph.
 *
 * @returns A constant reference to the state space graph.
 */
const Graph & NimLearner::getGraph() const {
  return g_;
}
