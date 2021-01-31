#include <unordered_map>
#include <vector>
#include <iostream>

using namespace std;

unordered_map<string, int> common_elems(unordered_map<string, int> mapA,
                                        unordered_map<string, int> mapB) {

    // your code here
    unordered_map<string, int> result;

    for(unordered_map<string, int>::iterator ita=mapA.begin(); ita!=mapA.end(); ita++){
      for(unordered_map<string, int>::iterator itb=mapB.begin(); itb!=mapB.end(); itb++){
        if(ita->first==itb->first){
          result[ita->first]+=ita->second;
          result[ita->first]+=itb->second;
          break;
        }
      }
    }

    for(unordered_map<string, int>::iterator it=result.begin(); it!=result.end(); it++){
      mapA.erase(it->first);
      mapB.erase(it->first);
    }

    return result;
}
