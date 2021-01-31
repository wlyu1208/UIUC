/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
  if  (curDim<Dim && 0<=curDim){
  if (first[curDim]<second[curDim])
     return true;
 if (first[curDim]>second[curDim])
   return false;
 else
   return first < second;
}
else
 return false;
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */

     double temp1; //current reuslt
     double temp2; //potential result
     for(int i=0; i<Dim; i++){
       temp1=temp1+((target[i]-currentBest[i])*(target[i]-currentBest[i]));
       temp2=temp2+((target[i]-potential[i])*(target[i]-potential[i]));
     } //get results for each
     if(temp1>temp2){  //if curr greater than poten, return true
       return true;
     }
     if(temp1<temp2){
       return false; //if poten greater than curr, reutrn false
     }
     else{
       return potential<currentBest; //if equal,return poten < curr
     }
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
     pt=newPoints; //set pt(point) value

     if(pt.size()==0){
       return; //if point size is 0, return
     }

     int l=0; //initialize left
     int dimen=0; //initiailize dimension

     Building(dimen, l, pt.size()-1); //build tree
     size=pt.size(); //re-set size
     root=buildroot(l, int(size-1)); //build tree on root
}

template <int Dim>
void KDTree<Dim>::Building(int dimen, int l, int r){
  int size=pt.size(); //get size of point
  if(l>=r || r>=size){
    return;
  } //if out of boundary, return
  if(l<r && r<size){ //if inside boundary
    int m=(l+r)/2;
    quickselect(dimen, l, r, m);
    Building((dimen+1)%Dim, l, m-1); //build left tree
    Building((dimen+1)%Dim, m+1, r); //build right tree
  }
}

template <int Dim>
void KDTree<Dim>::quickselect(int dimen, int l, int r, int m){
    if(l>=r){
      return;
    } //if left is greater than right, return
    int piv=partition(dimen, l, r, m); //split tree
    if(piv==m){
      return;
    } //if middle is equal to split, then return
    if(m<piv){
      quickselect(dimen, l, piv-1, m);
    } //traverse to left subtree
    else{
      quickselect(dimen, piv+1, r, m);
    } //traverse to right subtre
}

template <int Dim>
int KDTree<Dim>::partition(int dimen, int l, int r, int m){
  Point<Dim> pivot=pt[m]; //get middle point
  swap(m, r); //swap middle and right
  int idx=l; //set index to left

  for(int i=l; i<=r; i++){
    if(smallerDimVal(pt[i], pivot, dimen)){
      swap(idx, i); //swap if true
      idx++;
    }
}

  swap(r, idx); //swap right and index
  return idx;
}

template <int Dim>
void KDTree<Dim>::swap(int lwhat, int rwhat){
    Point<Dim> tem=pt[lwhat];
    pt[lwhat]=pt[rwhat];
    pt[rwhat]=tem; //swap two values
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::buildroot(int l, int r){
  if(l>r){
    return NULL;
  } //if out of boundary, return
  int m=(l+r)/2;
  if(m>=0 && m<int(pt.size()) ){ //if inside boundary
    KDTreeNode* subroot=new KDTreeNode(pt[m]); //create node
    subroot->left=buildroot(l, m-1); //create left subtree
    subroot->right=buildroot(m+1, r); //create right subtree
    return subroot; //return the root
  }
  return NULL; //otherwise return NULL
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
   pt=other.pt;
   size=other.size;
   int l=0;
   int r=int(size-1);
   root=buildroot(l, r); //copy pt, size, and root(left, right)
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
   if(this==&rhs){
     return *this;
   } //if add of rhs is same, return this
    deletion(this);
    pt=rhs.points;
    root=buildroot(0, rhs.points.size()-1);
    return *this; //else create again and return this
}


template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
   deletion(root);
}

template <int Dim>
void KDTree<Dim>::deletion(KDTreeNode* subroot){
  if(subroot!=NULL){
    deletion(subroot->left);
    deletion(subroot->right);
    delete subroot;
  }
  else{
    return;
  }
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
  Point<Dim> result;
  if(pt.size()==0){
    return Point<Dim>();
  }
  else{ //if size is 0, return empty
  int l=0;
  int dimen=0;
  int r=pt.size()-1; //set parameters
  result=neighboring(query, pt[(pt.size()-1)/2], l, r, dimen); //put result with necesary parameters
  }
  return result;
}

template <int Dim>
Point<Dim> KDTree<Dim>::neighboring(const Point<Dim>& query, const Point<Dim>& currBest, int l, int r, int dimen) const{
  int m=(l+r)/2; //get middle
  Point<Dim> result=currBest; //set result to currbest
  Point<Dim> original=pt[m]; //keep original point of middle

  if(l==r){ //if left and right same
    if(shouldReplace(query, currBest, pt[l])==false){ //check replsace
      result=currBest; //else set currbest as result and return
      return result;
    }else{
      result=pt[l];
      return result; //if necessary set result to point of left
    }
  }

  int new_dimen=(dimen+1)%Dim; //get new dimension
  bool flag=true; //set flag to true

  if(smallerDimVal(pt[m], query, dimen)==true){
    if(r>m){ //if right is greater than m
      result=neighboring(query, currBest, m+1, r, new_dimen);
      flag=false; //recursive set result and set flag to false
    }
  }

  if(smallerDimVal(query, pt[m], dimen)==true){
    if(l<m){ //if left is less than m
      result=neighboring(query, currBest, l, m-1, new_dimen);
      flag=true; //recursive set result and set flag to true
    }
  }

  if(shouldReplace(query, result, pt[m])==true){
  result=pt[m]; //if true then set result to middle point
  }

  int result_num=0; //initialize result number

  for(int i=0; i<Dim; i++){ //set loop within Dim
    result_num=result_num+( (query[i]-result[i])*(query[i]-result[i]) ); //get result number
  }

  if(( (original[dimen]-query[dimen])*(original[dimen]-query[dimen]) )<=result_num){ //if distance is less than result number
    if(l<m && flag==false){ //if left is less than middle and flag is false
      result=neighboring(query, result, l, m-1, new_dimen); //recursive result
    }
    if(r>m && flag==true){ //if right is greater than m and flag is true
      result=neighboring(query, result, m+1, r, new_dimen); //recursive result
    }
  }
  return result;
}
