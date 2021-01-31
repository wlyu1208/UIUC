#include "Node.h"

using namespace std;

void mergeList(Node *first, Node *second) {
  // your code here!
  if (first == NULL) return;
  if (second == NULL) return;
  Node * aptr = first;
  Node * bptr = second;
  while (aptr->next_ != NULL && bptr->next_ != NULL) {
     Node * tempa = aptr;
     aptr = aptr->next_;
     Node * tempb = bptr;
     bptr = bptr->next_;
     tempa->next_ = tempb;
     tempb->next_ = aptr;
  }
  if (aptr->data_==NULL && bptr->data_==NULL)
     return;

  if (aptr->next_ == NULL) {
     aptr->next_ = bptr;
  }
  else {
     Node * tempa = aptr;
     aptr = aptr->next_;
     tempa->next_ = bptr;
     bptr->next_ = aptr;
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
