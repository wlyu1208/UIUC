#ifndef BTREENODE_H
#define BTREENODE_H

#include <string>
#include <algorithm>
#include <vector>

struct BTreeNode {
    bool is_leaf_=true;
    bool visited = false;
    std::vector<int> elements_;
    std::vector<BTreeNode*> children_;
    //std::vector<int> result;
    BTreeNode() {}
    BTreeNode (std::vector<int> v) {
      this->elements_ = v;
    }
};

std::vector<int> traverse(BTreeNode* root);

#endif
