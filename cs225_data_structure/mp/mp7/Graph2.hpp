#include <queue>
#include <algorithm>
#include <string>
#include <list>
#include <utility>
#include <functional>
#include <iostream>

using namespace std;
/**
 * Returns an std::list of vertex keys that creates some shortest path between `start` and `end`.
 *
 * This list MUST include the key of the `start` vertex as the first vertex in the list, the key of
 * the `end` vertex as the last element in the list, and an ordered list of all vertices that must
 * be traveled along the shortest path.
 *
 * For example, the path a -> c -> e returns a list with three elements: "a", "c", "e".
 *
 * @param start The key for the starting vertex.
 * @param end   The key for the ending vertex.
 */
template <class V, class E>
std::list<std::string> Graph<V,E>::shortestPath(const std::string start, const std::string end) {
  std::list<std::string> path;

  unordered_map<string, double> d;
 unordered_map<string, bool> spt;
 unordered_map<string, string> p;

 for(auto it = vertexMap.begin(); it != vertexMap.end(); it++){
   d[(*it).second.key()] = 999999;
   p[(*it).second.key()] = "";
   spt[(*it).second.key()] = false;
 }
 d[start] = 0;
 (vertexMap.find(start)->second)["d"] = "0";

 for(unsigned i = 0; i < vertexMap.size(); i++){
   double min = 999999;
   string minkey = "";
   for(auto it=d.begin(); it != d.end(); it++){
     if(spt[(*it).first] == false && d[(*it).first] < min){
       minkey = (*it).first;
       min = (*it).second;
     }
   }
   spt[minkey] = true;
   auto el = incidentEdges(minkey);
   for(auto it = el.begin(); it != el.end(); it++){
     if((*it).get().dest().key() != minkey){
       auto to = (*it).get().dest().key();
       if( (*it).get().weight() + d[minkey] < d[to]){
         d[to] = (*it).get().weight() + d[minkey];
         p[to] = minkey;

       }

     }
     else{
       auto to = (*it).get().source().key();
       if( (*it).get().weight() + d[minkey] < d[to]){
         d[to] = (*it).get().weight() + d[minkey];
         p[to] = minkey;
       }
     }
   }
 }
 string asdf = end;
 while(asdf != start){
   path.push_front(asdf);
   asdf = p[asdf];
 }
 path.push_front(asdf);

  return path;
}
