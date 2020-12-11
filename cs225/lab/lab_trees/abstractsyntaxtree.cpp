#include "abstractsyntaxtree.h"

/**
 * Calculates the value from an AST (Abstract Syntax Tree). To parse numbers from strings, please use std::stod
 * @return A double representing the calculated value from the expression transformed into an AST
 */
using namespace std;

double AbstractSyntaxTree::eval() const {
    // @TODO Your code goes here...
    Node* temp=root;
    return eval(temp);
}

double AbstractSyntaxTree::eval(Node* subRoot) const{
  if(subRoot==NULL){
    return 0;
  }
  if(subRoot->left==NULL && subRoot->right==NULL){
  return stod(subRoot->elem);
}
double result=0;
double x=eval(subRoot->left);
double y=eval(subRoot->right);
if(subRoot->elem=="+"){
  result=x+y;
  return result;
}
if(subRoot->elem=="-"){
  result=x-y;
  return result;
}
if(subRoot->elem=="*"){
  result=x*y;
  return result;
}
if(subRoot->elem=="/"){
  result=x/y;
  return result;
}
  else
  return -1;
}
