#include "Node.h"

using namespace std;

Node *listUnion(Node *first, Node *second) {
  // your code here
  Node* result=NULL;
  while(first!=NULL && second !=NULL){
    if(first->data_>second->data_){
      result=second;
      second=second->next_;
    }
    else if(first->data_<second->data_){
      result=first;
      first=first->next_;
    }
    else if(first->data_==second->data_){
      first=first->next_;
    }
    result=result->next_;
  }
  return result;
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
