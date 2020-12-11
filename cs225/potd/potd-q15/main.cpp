#include "potd.h"
#include <iostream>
using namespace std;

int main() {
  // Test 1: An empty list
  Node * head = NULL;
  cout << stringList(head) << endl;

  // Test 2: A list with exactly one node
  Node * head1 = new Node();
cout << stringList(head1) << endl;

head1 = NULL;
delete head1;
head1 = NULL;

  // Test 3: A list with more than one node
  Node * head11 = new Node();
    Node * head2= new Node();
cout << stringList(head11) << stringList(head2)<< endl;
head1 = NULL;
delete head1;
head1 = NULL;

  return 0;
}
