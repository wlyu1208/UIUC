#include "HuffmanNode.h"
#include "HuffmanUtils.h"
#include <string>
#include <vector>

using std::string;
using namespace std;
/**
 * binaryToString
 *
 * Write a function that takes in the root to a huffman tree
 * and a binary string.
 *
 * Remember 0s in the string mean left and 1s mean right.
 */

string binaryToString(string binaryString, HuffmanNode* huffmanTree) {
    /* TODO: Your code here */
    string result="";
    HuffmanNode* temp=huffmanTree;
    for(unsigned long i=0; i<binaryString.size(); i++){
      if(binaryString[i]=='0'){
        temp=temp->left_;
      }
      else{
        temp=temp->right_;
      }
      if(temp->left_==NULL && temp->right_==NULL){
        result += temp->char_;
        temp=huffmanTree;
      }
    }
    return result;
}

/**
 * stringToBinary
 *
 * Write a function that takes in the root to a huffman tree
 * and a character string. Return the binary representation of the string
 * using the huffman tree.
 *
 * Remember 0s in the binary string mean left and 1s mean right
 */

 void encode(HuffmanNode * node, std::vector<char> &c, std::vector<vector<char>> & path, std::vector<char> & p){
     if(node == NULL) return;
     if(node->left_ == NULL && node->right_ == NULL){
         c.push_back(node->char_);
         path.push_back(p);
     }
     p.push_back('0');
     encode(node->left_, c, path, p);
     p.pop_back();
     p.push_back('1');
     encode(node->right_, c, path, p);
     p.pop_back();
 }
 using namespace std;
 string stringToBinary(string charString, HuffmanNode* huffmanTree) {
     std::vector<char> c;
     std::vector<vector<char>> path;
     std::vector<char> p;

     encode(huffmanTree, c, path, p);

     std::string s = "";
     for(int i=0; i<charString.size(); i++){
         for(int j=0; j<c.size(); j++){
             if(c[j] == charString[i]){
                 for(int k=0; k<path[j].size(); k++){
                     s += path[j][k];
                     //cout<<"fuck"<<endl;
                 }
             }
         }
         /*
         for(int j=0; j<path[i].size(); j++){

         }
         */
     }
     return s;
}
