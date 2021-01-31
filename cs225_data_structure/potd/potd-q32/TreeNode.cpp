#include "TreeNode.h"
#include <cstdlib>

int getHeight(TreeNode * root){
  if(root == NULL) return 0;
  else{
    int left = getHeight(root->left_);
    int right = getHeight(root->right_);

    if(left > right) return left +1;
    else return right + 1;

  }
}

bool isHeightBalanced(TreeNode* root) {
  bool cur;
  if(root == NULL) return true;

  if(abs(getHeight(root->left_) - getHeight(root->right_)) <= 1) cur =  true;
  else cur = false;
  bool left = isHeightBalanced(root->left_);
  bool right = isHeightBalanced(root->right_);
  // your code here
  return cur && left && right;
}

void find(TreeNode * root, int level, int &maxLevel, TreeNode *&res){
  if (root != NULL) {
        find(root->left_, ++level, maxLevel, res);

        // Update level and resue
        if (level > maxLevel && !isHeightBalanced(root))
        {
            res = root;
            maxLevel = level;
        }

        find(root->right_, level, maxLevel, res);
    }
}

TreeNode* findLastUnbalanced(TreeNode* root) {
  // your code here
  int level = getHeight(root);
  int maxLevel = level;
  TreeNode * node = NULL;
  find(root, level, maxLevel, node);

  return node;
}

void deleteTree(TreeNode* root)
{
  if (root == NULL) return;
  deleteTree(root->left_);
  deleteTree(root->right_);
  delete root;
  root = NULL;
}
