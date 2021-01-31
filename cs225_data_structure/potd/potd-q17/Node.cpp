#include "Node.h"

using namespace std;

void sortList(Node **head){
  // your code here!
  Node *original = *head;
  *head=nullptr;

  while(original){
    Node **current=&original;
    Node **next=&original->next_;
    bool swap=false;

    while(*next){
      if((*next)->data_ < (*current)->data_){
        std::swap(*current, *next);
        std::swap((*current)->next_, (*next)->next_);
        current=&(*current)->next_;
        swap=true;
      }
      else{
        current=next;
        next=&(*next)->next_;
      }
    }
    *next=*head;
    if(swap){
      *head=*current;
      *current=nullptr;
    }
    else{
      *head=original;
      break;
    }
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
