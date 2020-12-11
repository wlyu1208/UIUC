#include "potd.h"
#include <iostream>

using namespace std;

void insertSorted(Node **head, Node *insert) {
  // your code here!
  if(head==NULL){
    head=&insert;
    return;
  }
  Node* temp = *head;
  if(temp->data_>=insert->data_){
    insert->next_= temp;
    *head=insert;
  }
  else{
    Node* prev = *head;
    temp = temp->next_;
    while(temp!=NULL){
      if(temp->data_ >= insert->data_){
        insert->next_=temp;
        prev->next_=insert;
        return;
      }
      temp=temp->next_;
      prev=prev->next_;
    }
    prev->next_=insert;
    insert->next_=NULL;
  }
}
