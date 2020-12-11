#include "Stack.h"

// `int size()` - returns the number of elements in the stack (0 if empty)
int Stack::size() const {
  return nodesize;
}

// `bool isEmpty()` - returns if the list has no elements, else false
bool Stack::isEmpty() const {
  if(nodesize==0){
    return true;
  }
  return false;
}

// `void push(int val)` - pushes an item to the stack in O(1) time
void Stack::push(int value) {
  node* temp=new node;
  temp->data=value;
  temp->next=head;
  head=temp;
  nodesize++;
}

// `int pop()` - removes an item off the stack and returns the value in O(1) time
int Stack::pop() {
  if(nodesize==0){
    return 0;
  }
  node* temp=head;
  head=head->next;
  int temp2=temp->data;
  delete temp;
  nodesize--;
  return temp2;
}
