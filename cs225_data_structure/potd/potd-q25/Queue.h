#ifndef _QUEUE_H
#define _QUEUE_H

#include <cstddef>

class Queue {
    public:
        Queue();
        int size() const;
        bool isEmpty() const;
        void enqueue(int value);
        int dequeue();
    private:
        struct node{
          int data;
          node* next;
        };
        node* head;
        node* tail;
        int nodesize;
};

#endif
