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




template  class Queue : private Semaphore : private Mutex {
private:
  QueueEntry* head;
  QueueEntry* tail;

public:
  Queue(void) {
    head = tail = (QueueEntry*)0;
  }

  void Add(const T& item_value) {
    Lock();
    if (tail == QueueEntry*)0)
      head = tail = new QueueEntry(item_value);
    else {
      tail->next = new QueueEntry(item_value);
      tail = tail->next;
    }
    Unlock();
    Post(); // wake up any waiting threads
  }

  T Wait(void) {
    Semaphore::Wait(); // wait for something to show up
    Lock();
    T value = head->value;
    QueueEntry* old = head;
    head = head->next;
    delete old;
    if (head == (QueueEntry*)0)
      tail = (QueueEntry*)0;
    Unlock();
    return value;
  }
};



//Any number of threads may wait on a queue, and as data is added to the queue
//they will be woken up one-at-a-time. If no threads are waiting when data is
//added to the queue, any thread coming along at a later time will simply pick
//up with waiting data immediately.