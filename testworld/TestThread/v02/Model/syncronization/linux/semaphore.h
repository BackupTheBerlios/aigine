/******************************************************
*               The Semaphore Class
* Copyright (c) 1995 Jim Frost
* All Rights Reserved
* Last changed February 17, 1995
*******************************************************/

//The semaphore synchronization object is used so that one thread may allow one or more other threads to pass a particular point at a particular time.


//The interface to the Semaphore class is:

class Semaphore {
public:
  Semaphore(void);
  Semaphore(int available);
  ~Semaphore(void);
  void Wait(void);
  void Post(void);
  void Post(int how_many);
};
