
class ListIterator : public std::iterator<std::bidirectional_iterator_tag, T> {
  private:
    ListNode* position_;

  public:
    ListIterator() : position_(nullptr) { }
    ListIterator(ListNode* x) : position_(x) { }

    // Pre-Increment, ++iter
    ListIterator& operator++() {
        //: graded in MP3.1
        position_=position_->next;
        return *this;
    }

    // Post-Increment, iter++
    ListIterator operator++(int) {
        ListNode* temp = position_;
        position_ = position_->next;
        return ListIterator(temp);
    }

    // Pre-Decrement, --iter
    ListIterator& operator--() {
        position_ = position_->prev;
        return *this;
    }

    // Post-Decrement, iter--
    ListIterator operator--(int) {
        // : graded in MP3.1
        ListNode* temp = position_;
        position_=position_->prev;
        return ListIterator(temp);
    }

    bool operator==(const ListIterator& rhs) {
        return !(*this != rhs);
    }

    bool operator!=(const ListIterator& rhs) {
        // : graded in MP3.1
        ListNode* compare=position_;
        ListNode* compare2=rhs.position_;
        while(compare!=NULL ||compare2!=NULL){
          if(compare==NULL && compare2!=NULL){
            return true;
          }
          if(compare!=NULL && compare2==NULL){
            return true;
          }
          if(compare->data != compare2->data){
            return true;
          }
          if(compare==NULL || compare2== NULL){
            return false;
          }
          else{
            compare=compare->next;
            compare2=compare2->next;
          }
        }
        return false;
    }

    const T& operator*() {
        return position_->data;
    }

    const T* operator->() {
        return &(position_->data);
    }
};
