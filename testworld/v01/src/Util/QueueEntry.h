/*******************************************************************
*        Portable Thread Synchronization Using C++
*
* copy from http://world.std.com/~jimf/papers/c++sync/c++sync.html#Introduction
*******************************************************************/


//Semaphores are most useful when threads are in a producer/consumer
//relationship, as in the case of a queue. The following implements
//a queue template class using the Semaphore class:

template  class QueueEntry : public T {
public:
  T value;
  QueueEntry* next;

  QueueEntry(const T& item_value) {
    value = item_value;
    next = (QueueEntry*)0;
  }
};
