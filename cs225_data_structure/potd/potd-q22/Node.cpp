#include "Node.h"

using namespace std;

Node *listIntersection(Node *first, Node *second) {
  // your code here
Node* result=new Node();
Node* curr  = result;
Node* a = first;
Node* b = second;
Node* prev=result;

while(a!=NULL){
  if(a->data_ > b->data_){
    b=b->next_;
    continue;
  }
  else if(a->data_ == b->data_){
    if(a->data_ != prev->data_){
      curr->data_=a->data_;
      prev=curr;
      curr->next_=new Node();
      curr=curr->next_;
      a=a->next_;
      b=b->next_;
    }
  }else{
    a=a->next_;
  }
}
delete curr;
curr=NULL;
prev->next_=NULL;
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
