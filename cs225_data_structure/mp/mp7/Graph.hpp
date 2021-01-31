#include "Graph.h"
#include "Edge.h"
#include "Vertex.h"

#include <string>
#include <iostream>

using namespace std;
/**
* @return The number of vertices in the Graph
*/
template <class V, class E>
unsigned int Graph<V,E>::size() const {
  // TODO: Part 2
  return vertexMap.size();
}


/**
* @return Returns the degree of a given vertex.
* @param v Given vertex to return degree.
*/
template <class V, class E>
unsigned int Graph<V,E>::degree(const V & v) const {
  // TODO: Part 2
  return adjList[v.key()].size();
}


/**
* Inserts a Vertex into the Graph by adding it to the Vertex map and adjacency list
* @param key The key of the Vertex to insert
* @return The inserted Vertex
*/
template <class V, class E>
V & Graph<V,E>::insertVertex(std::string key) {
  // TODO: Part 2
  V & v = *(new V(key));
  vertexMap.insert({key, v});
  list<edgeListIter> lis;
  adjList.insert({key, lis});
  return v;
}


/**
* Removes a given Vertex
* @param v The Vertex to remove
*/
template <class V, class E>
void Graph<V,E>::removeVertex(const std::string & key) {
  // TODO: Part 2
  auto al = incidentEdges(key);

  for(auto it = al.begin(); it != al.end(); it++){
      removeEdge((*it).get().source().key(), (*it).get().dest().key());
  }
  vertexMap.erase(key);
  adjList.erase(key);
}


/**
* Inserts an Edge into the adjacency list
* @param v1 The source Vertex
* @param v2 The destination Vertex
* @return The inserted Edge
*/
template <class V, class E>
E & Graph<V,E>::insertEdge(const V & v1, const V & v2) {
  // TODO: Part 2
  E & e = *(new E(v1, v2));
  edgeList.push_front(e);

  list<edgeListIter> el=adjList.find(v1.key())->second;
  edgeListIter it=edgeList.begin();
  el.push_back(it);
  adjList[v1.key()]=el;

  list<edgeListIter> el2=adjList.find(v2.key())->second;
  edgeListIter it2=edgeList.begin();
  el2.push_back(it2);
  adjList[v2.key()]=el2;

  return e;
}


/**
* Removes an Edge from the Graph
* @param key1 The key of the ource Vertex
* @param key2 The key of the destination Vertex
*/
template <class V, class E>
void Graph<V,E>::removeEdge(const std::string key1, const std::string key2) {
  // TODO: Part 2
  V &  v1 = vertexMap.find(key1)->second;
  V &  v2 = vertexMap.find(key2)->second;
  E e = E(v1, v2);
  edgeListIter it;
  //int ind = -1;
  list<E_byRef> newel;
  for(auto iter = edgeList.begin(); iter != edgeList.end(); iter++){
    //ind++;
    if( (*iter).get() == e){
      it = iter;
    }
    else{
      newel.push_back(*iter);
    }
  }
  list<edgeListIter> el = adjList.find(key1)->second;
  el.remove(it);
  adjList.find(key1)->second = el;

  list<edgeListIter> el2 = adjList.find(key2)->second;
  el2.remove(it);
  adjList.find(key2)->second = el2;
  edgeList = newel;
}


/**
* Removes an Edge from the adjacency list at the location of the given iterator
* @param it An iterator at the location of the Edge that
* you would like to remove
*/
template <class V, class E>
void Graph<V,E>::removeEdge(const edgeListIter & it) {
  V   v1 = (*it).get().source();
  V   v2 = (*it).get().dest();
  string  key1 = v1.key();
  string  key2 = v2.key();
  E  e = E(v1, v2);
  list<E_byRef> newel;
  for(auto iter = edgeList.begin(); iter != edgeList.end(); iter++){
    //ind++;
    if( (*iter).get() == e){
      //it = iter;
    }
    else{
      newel.push_back(*iter);
    }
  }

  list<edgeListIter> el = adjList.find(key1)->second;
  el.remove(it);
  adjList.find(key1)->second = el;

  list<edgeListIter> el2 = adjList.find(key2)->second;
  el2.remove(it);
  adjList.find(key2)->second = el2;
  edgeList = newel;
}


/**
* @param key The key of an arbitrary Vertex "v"
* @return The list edges (by reference) that are adjacent to "v"
*/
template <class V, class E>
const std::list<std::reference_wrapper<E>> Graph<V,E>::incidentEdges(const std::string key) const {
  // TODO: Part 2
  list<edgeListIter> el=adjList.find(key)->second;
  list<std::reference_wrapper<E>> edges;

  for(auto it=el.begin(); it!=el.end(); it++){
    edges.push_back(**it);
  }
  return edges;
}


/**
* Return whether the two vertices are adjacent to one another
* @param key1 The key of the source Vertex
* @param key2 The key of the destination Vertex
* @return True if v1 is adjacent to v2, False otherwise
*/
template <class V, class E>
bool Graph<V,E>::isAdjacent(const std::string key1, const std::string key2) const {
  // TODO: Part 2
  V& v1=vertexMap.find(key2)->second;
  auto al=incidentEdges(key1);
  for(auto it=al.begin(); it!=al.end(); it++){
    if((*it).get().dest()==v1)
    return true;
  }
  return false;
}
