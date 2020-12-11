#include "Queue.h"

Queue::Queue() {
head=NULL;
tail=NULL;
nodesize=0;
}

// `int size()` - returns the number of elements in the stack (0 if empty)
int Queue::size() const {
  return nodesize;
}

// `bool isEmpty()` - returns if the list has no elements, else false
bool Queue::isEmpty() const {
  if(nodesize==0){
    return true;
  }
  else{
    return false;
  }
}

// `void enqueue(int val)` - enqueue an item to the queue in O(1) time
void Queue::enqueue(int value) {
  if(head==NULL){
    node* temp= new node;
    temp->data=value;
    head=temp;
    tail=temp;
    temp->next=NULL;
  }
  else{
    node* temp=new node;
    temp->data=value;
    node* temp2= tail;
    tail=temp;
    temp2->next=tail;
    tail->next=NULL;
  }
  nodesize++;
}

// `int dequeue()` - removes an item off the queue and returns the value in O(1) time
int Queue::dequeue() {
  if(nodesize==0){
    return 0;
  }
  else{
    node* temp=head;
    head=head->next;
    int tempnum=temp->data;
    delete temp;
    nodesize--;
    return tempnum;
  }
}
