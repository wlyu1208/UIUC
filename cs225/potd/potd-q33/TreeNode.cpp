#include "TreeNode.h"
#include <algorithm>


void rightRotate(TreeNode* root) {

    // Your code here
    TreeNode* leftoft = root->left_;
    TreeNode* leftrightoft = root->left_->right_;

    leftoft->right_ = root;
    root->left_ = leftrightoft;

    leftrightoft->parent_ = root;
    leftoft->parent_ = NULL;
    root->parent_ = leftoft;

    root = leftoft;
}


void leftRotate(TreeNode* root) {

    // your code here
    TreeNode* rightoft = root->right_;
    TreeNode* rightleftoft = root->right_->left_;

    rightoft->left_ = root;
    root->right_ = rightleftoft;

    rightleftoft->parent_ = root;
    rightoft->parent_ = NULL;
    root->parent_ = rightoft;

    root = rightoft;
}


void deleteTree(TreeNode* root)
{
  if (root == NULL) return;
  deleteTree(root->left_);
  deleteTree(root->right_);
  delete root;
  root = NULL;
}
