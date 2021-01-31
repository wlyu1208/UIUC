/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V AVLTree<K, V>::find(const K& key) const
{
    return find(root, key);
}

template <class K, class V>
V AVLTree<K, V>::find(Node* subtree, const K& key) const
{
    if (subtree == NULL)
        return V();
    else if (key == subtree->key)
        return subtree->value;
    else {
        if (key < subtree->key)
            return find(subtree->left, key);
        else
            return find(subtree->right, key);
    }
}

template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node*& t)
{
    functionCalls.push_back("rotateLeft"); // Stores the rotation name (don't remove this)
    // your code here
    Node* temp=t->right;
    t->right=temp->left;
    temp->left=t;
    t->height=max(heightOrNeg1(t->left), heightOrNeg1(t->right))+1;
    temp->height=max(heightOrNeg1(temp->left), heightOrNeg1(temp->right))+1;
    t=temp;
}

template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node*& t)
{
    functionCalls.push_back("rotateLeftRight"); // Stores the rotation name (don't remove this)
    // Implemented for you:
    rotateLeft(t->left);
    rotateRight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRight(Node*& t)
{
    functionCalls.push_back("rotateRight"); // Stores the rotation name (don't remove this)
    // your code here
    Node* temp=t->left;
    t->left=temp->right;
    temp->right=t;
    t->height=max(heightOrNeg1(t->left), heightOrNeg1(t->right))+1;
    temp->height=max(heightOrNeg1(temp->left), heightOrNeg1(temp->right))+1;
    t=temp;
}

template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node*& t)
{
    functionCalls.push_back("rotateRightLeft"); // Stores the rotation name (don't remove this)
    // your code here
    rotateRight(t->right);
    rotateLeft(t);
}

template <class K, class V>
void AVLTree<K, V>::rebalance(Node*& subtree)
{
    // your code here
    int bal=heightOrNeg1(subtree->left)-heightOrNeg1(subtree->right);
    Node* L=subtree->left;
    Node* R=subtree->right;
    if(bal<-1){
      int Rbal=heightOrNeg1(R->right)-heightOrNeg1(R->left);
      if(Rbal==1){
        rotateLeft(subtree);
      }
      else{
        rotateRightLeft(subtree);
      }
    }
    if(bal>1){
      int Lbal=heightOrNeg1(L->left)-heightOrNeg1(L->right);
      if(Lbal==1){
        rotateRight(subtree);
      }
      else{
        rotateLeftRight(subtree);
      }
    }
    subtree->height=max(heightOrNeg1(subtree->left), heightOrNeg1(subtree->right))+1;
}

template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);
}

template <class K, class V>
void AVLTree<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    // your code here
    if(subtree==NULL){
      subtree=new Node(key, value);
      return;
    }
    if(key<subtree->key){
      insert(subtree->left, key, value);
    }
    if(key>subtree->key){
      insert(subtree->right, key, value);
    }
    subtree->height=max(heightOrNeg1(subtree->left), heightOrNeg1(subtree->right))+1;
    rebalance(subtree);
}

template <class K, class V>
void AVLTree<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
void AVLTree<K, V>::remove(Node*& subtree, const K& key)
{
    if (subtree == NULL)
        return;

    if (key < subtree->key) {
        // your code here
        remove(subtree->left, key);
    } else if (key > subtree->key) {
        // your code here
        remove(subtree->right, key);
    } else {
        if (subtree->left == NULL && subtree->right == NULL) {
            /* no-child remove */
            // your code here
            delete subtree;
            subtree=NULL;
        } else if (subtree->left != NULL && subtree->right != NULL) {
            /* two-child remove */
            // your code here
            Node* temp=subtree->left;
            while(temp->right!=NULL){
              temp=temp->right;
            }
          swap(subtree, temp);
          remove(subtree->left, key);
        } else {
            /* one-child remove */
            // your code here
            Node* temp=subtree->left;
            Node* temp2=subtree->right;
            delete subtree;
            if(temp!=NULL){
              subtree=temp;
            }
            else{
              subtree=temp2;
            }
        }
        // your code here
    }
    if(subtree!=NULL){
      rebalance(subtree);
    }
    else{
      return;
    }
}
