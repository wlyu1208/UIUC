#include <vector>
#include "BTreeNode.h"


BTreeNode* find(BTreeNode* root, int key) {
  unsigned numelement = root->elements_.size();
  unsigned numchild = root->children_.size();
  BTreeNode* temp = NULL;

  for (unsigned i = 0; i < numelement; i++) {
    if (root->elements_[i] == key) {
      temp = root;
      return temp;
    }
  }

  for (unsigned j = 0; j < numchild; j++) {
    temp = find(root->children_[j], key);
    if (temp != NULL) {
      break;
    }
  }

  return temp;
}
//
BTreeNode* findc(std::vector<BTreeNode*> children_, int key) {
  for (unsigned i = 0; i < children_.size(); i++) {
    unsigned numelec = children_[i]->elements_.size();
    for (unsigned j = 0; j < numelec; j++) {
      if (children_[i]->elements_[j] == key) {
        return children_[i];
      }
    }
    BTreeNode* temp = children_[i];
    findc(temp->children_, key);
  }
}
