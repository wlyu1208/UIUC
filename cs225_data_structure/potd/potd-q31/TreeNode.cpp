#include "TreeNode.h"
#include <stdio.h>
#include <stdlib.h>

bool isHeightBalanced(TreeNode* root) {
  // your code here
  int lh, rh;
   if(root==NULL){
     return true;
   }
   lh=height(root->left_);
   rh=height(root->right_);
   if(abs(lh-rh)<=1 && isHeightBalanced(root->left_) && isHeightBalanced(root->right_)){
     return 1;
   }
   return 0;
}

void deleteTree(TreeNode* root)
{
  if (root == NULL) return;
  deleteTree(root->left_);
  deleteTree(root->right_);
  delete root;
  root = NULL;
}

int height(TreeNode* root){
  if(root==NULL){
    return 0;
  }
  return 1+max(height(root->left_), height(root->right_));
}

int max(int a, int b){
  return (a>=b)? a: b;
}
