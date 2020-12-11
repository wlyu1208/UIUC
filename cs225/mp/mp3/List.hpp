/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */
#include <iostream>
#include <queue>

using namespace std;

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in MP3.1
  return List<T>::ListIterator(head_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: graded in MP3.1
  return List<T>::ListIterator(tail_->next);
}

/**
 * Destroys the current List. This function should ensure that
 * memory does not leak on destruction of a list.
 */
template <typename T>
List<T>::~List() {
  /// @todo Graded in MP3.1
  _destroy();
}

/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  /// @todo Graded in MP3.1
  ListNode* deleted= head_;
  ListNode* temp=head_;
  while(deleted!=nullptr){
    temp=deleted;
    deleted=temp->next;
    delete temp;
  }
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  /// @todo Graded in MP3.1
  ListNode* front_ = new ListNode(ndata);

  if(head_==nullptr){
    head_=front_;
    tail_=head_;
  }
  else{
    head_->prev=front_;
    front_->next=head_;
    front_->prev=nullptr;
    head_=front_;
  }
  length_++;
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  /// @todo Graded in MP3.1
  ListNode* back_= new ListNode(ndata);

  if(head_==nullptr){
    head_=back_;
    tail_=head_;
  }
  else{
    tail_->next=back_;
    back_->prev=tail_;
    back_->next=nullptr;
    tail_=back_;
  }
  length_++;
}

/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  /// @todo Graded in MP3.1
  if(startPoint==nullptr){
    return;
  }
  if(endPoint==nullptr){
    return;
  }
  ListNode* curr=startPoint;
  ListNode* temp=startPoint;
  ListNode* begin_prev=startPoint->prev;

  while(curr!=endPoint){
    temp=curr->next;
    curr->next=curr->prev;
    curr->prev=temp;
    curr=temp;
  }
  temp=curr->next;
  startPoint->next=curr->next;
  curr->next=curr->prev;

  if(startPoint==head_){
    curr->prev=nullptr;
  }
  else{
    begin_prev->next=curr;
  }

  if(endPoint!=tail_){
    temp->prev=startPoint;
  }
  temp=endPoint;
  endPoint=startPoint;
  startPoint=temp;
}

/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
  /// @todo Graded in MP3.
  if(head_==nullptr){
    return ;
  }

  int count=n;
  int flag=1;
  queue<ListNode*> start;
  queue<ListNode*> end;

  ListNode* first=head_;
  ListNode* last=head_;

  while(last->next!=nullptr && first->next!=nullptr){
    count=n;
    first=last;
    for(int i=0; i<count-1; i++){
      if(last->next!=nullptr){
        last=last->next;
      }
    }
    start.push(first);
    end.push(last);
    if(last->next!=nullptr){
    last=last->next;
  }
}
  while(!start.empty() && !end.empty()){
  first=start.front();
  last=end.front();
  reverse(first, last);
  start.pop();
  end.pop();
  if(flag==1){
    head_=first;
    flag=0;
  }
  }
tail_=last;
}
/**
 * Modifies the List using the waterfall algorithm.
 * Every other node (starting from the second one) is removed from the
 * List, but appended at the back, becoming the new tail. This continues
 * until the next thing to be removed is either the tail (**not necessarily
 * the original tail!**) or NULL.  You may **NOT** allocate new ListNodes.
 * Note that since the tail should be continuously updated, some nodes will
 * be moved more than once.
 */
template <typename T>
void List<T>::waterfall() {
  /// @todo Graded in MP3.1
  ListNode* start=head_;
  ListNode* temp=head_->next;

  if(start==nullptr){
    return;
  }
  while(temp!=tail_){
    start->next=temp->next;
    start->next->prev=start;
    tail_->next=temp;
    temp->prev=tail_;
    temp->next=nullptr;
    tail_=temp;
    temp=start->next->next;
    start=start->next;
  }
}

/**
 * Splits the given list into two parts by dividing it at the splitPoint.
 *
 * @param splitPoint Point at which the list should be split into two.
 * @return The second list created from the split.
 */
template <typename T>
List<T> List<T>::split(int splitPoint) {
    if (splitPoint > length_)
        return List<T>();

    if (splitPoint < 0)
        splitPoint = 0;

    ListNode * secondHead = split(head_, splitPoint);

    int oldLength = length_;
    if (secondHead == head_) {
        // current list is going to be empty
        head_ = NULL;
        tail_ = NULL;
        length_ = 0;
    } else {
        // set up current list
        tail_ = head_;
        while (tail_ -> next != NULL)
            tail_ = tail_->next;
        length_ = splitPoint;
    }

    // set up the returned list
    List<T> ret;
    ret.head_ = secondHead;
    ret.tail_ = secondHead;
    if (ret.tail_ != NULL) {
        while (ret.tail_->next != NULL)
            ret.tail_ = ret.tail_->next;
    }
    ret.length_ = oldLength - splitPoint;
    return ret;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
  /// @todo Graded in MP3.2
  if(splitPoint>length_){
    return start;
  }
  if(start==nullptr){
    return NULL;
  }
  int count=splitPoint;
  ListNode* temp=start;
  for(int i=0; i<count; i++){
    temp=temp->next;
  }
  temp->prev->next=nullptr;
  temp->prev=nullptr;
  return temp;
}

/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
  /// @todo Graded in MP3.2
  if(first==nullptr && second==nullptr){
    return NULL;
  }
  if(first==nullptr){
    return second;
  }
  if(second==nullptr){
    return first;
  }

  int flag=0;
  ListNode* temp1=first;
  ListNode* temp2=second;
  ListNode* result=nullptr;

  if(temp2->data < temp1->data){
    flag=1;
    result=temp2;
    temp2=temp2->next;
  }
  else{
    flag=0;
    result=temp1;
    temp1=temp1->next;
  }

  while(temp1!=nullptr || temp2!=nullptr){
    if(temp2==nullptr || (temp1!=nullptr && (temp1->data < temp2->data))){
      result->next=temp1;
      temp1=temp1->next;
      result=result->next;
    }
    else{
      result->next=temp2;
      temp2=temp2->next;
      result=result->next;
    }
  }
  if(flag==0){
    return first;
  }
  else{
    return second;
  }
}

/**
 * Sorts the current list by applying the Mergesort algorithm.
 */
template <typename T>
void List<T>::sort() {
    if (empty())
        return;
    head_ = mergesort(head_, length_);
    tail_ = head_;
    while (tail_->next != NULL)
        tail_ = tail_->next;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  /// @todo Graded in MP3.2
  if(chainLength==0 || chainLength==1){
    return start;
  }
  if(start==nullptr){
    return NULL;
  }
  int middle=chainLength/2;
  ListNode* median=split(start, middle);
  int second_length=chainLength-middle;
  return merge(mergesort(start, middle), mergesort(median, second_length));
}
