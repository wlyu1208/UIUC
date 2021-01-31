
#include "potd.h"
#include <iostream>
#include <string>

using namespace std;

string stringList(Node *head) {
    // your code here!

    if (head != NULL) {
      string result;
      int count = 0;
      while (head != NULL) {
        Node* tn = head->next_;
        if (head -> next_ == NULL){
          int mydata = head->data_;
          string s = to_string(mydata);
          string temp = "Node " + to_string(count) + ": " + s;
          result.append(temp);
          count++;
        }
        else{
          int mydata = head->data_;
          string s = to_string(mydata);
          string temp = "Node " + to_string(count) + ": " + s + " -> ";
          result.append(temp);
          count++;
        }
        head = tn;
      }
    return result;
    }
    else {
      return "Empty list";
    }
}
