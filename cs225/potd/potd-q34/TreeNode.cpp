#include "TreeNode.h"

int getBalance(TreeNode * node){
  int leftHeight = getHeight(node->left_);
  int rightHeight = getHeight(node->right_);

  return rightHeight - leftHeight;
}

TreeNode::RotationType balanceTree(TreeNode*& subroot) {
   // Your code here
  int balance = getBalance(subroot);
  if(balance < -1){
    if(getBalance(subroot->left_) <= -1){
      return TreeNode::right;
    }
    else{
      return TreeNode::leftRight;
    }
  }
  else if(balance > 1){
    if(getBalance(subroot->right_) >= 1){
      return TreeNode::left;
    }
    else{
      return TreeNode::rightLeft;
    }

  }
  return TreeNode::left;
}
