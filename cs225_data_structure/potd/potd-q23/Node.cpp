#include "Node.h"

using namespace std;

Node *listSymmetricDifference(Node *first, Node *second) {
  // your code here
  Node* a = first;
  Node* b = second;
  Node* b_const = second;

  if(a==NULL && b==NULL){
    return NULL;
  }

  Node* result=new Node;
  result=NULL;

  if(a==NULL && b!=NULL){
    result = b;
    result->next_=NULL;
    return result;
  }
  if(a!=NULL && b==NULL){
    result = a;
    result->next_=NULL;
    return result;
  }
  
}

Node::Node() {
    numNodes++;
}

Node::Node(Node &other) {
    this->data_ = other.data_;
    this->next_ = other.next_;
    numNodes++;
}

Node::~Node() {
    numNodes--;
}

int Node::numNodes = 0;
