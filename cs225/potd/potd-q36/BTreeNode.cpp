#include <vector>
#include "BTreeNode.h"

std::vector<int> v;

std::vector<int> traverse(BTreeNode* root) {
    // your code here
//    std::vector<int> v;

    if(root->is_leaf_ == true){
      for(unsigned k = 0; k < root->elements_.size(); k++){
        v.push_back(root->elements_[k]);
      }
      root->visited = true;
      return v;
    }

    int j = 0;

    for(unsigned i = 0; i < root->children_.size() - 1; i++){
      if(!root->children_[i]->visited)
        traverse(root->children_[i]);
      if(root->is_leaf_ == false){
        v.push_back(root->elements_[j++]);
      }
      if(root->children_[i + 1] != NULL){
        traverse(root->children_[i + 1]);
      }
    }

    return v;
}
