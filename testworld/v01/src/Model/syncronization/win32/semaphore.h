/**********************************************************
*Win32 (Windows NT and Windows 95) Semaphore Class Implementation
*
**********************************************************/


class Semaphore {
private:
  HANDLE semaphore;

public:
  Semaphore(void) {
    CreateSemaphore((void *)0, 0, 0x7ffffff, (void*)0)
  }

  Semaphore(int available) {
    CreateSemaphore((void *)0, available, 0x7ffffff, (void*)0)
  }

  ~Semaphore(void) {
    CloseHandle(semaphore);
  }

  void Wait(void) {
    WaitForSingleObject(semaphore, INFINITE);
  }

  void Post(void) {
    ReleaseSemaphore(semaphore, 1, (void*)0);
  }

  void Post(int how_many) {
    ReleaseSemaphore(semaphore, how_many, (void*)0);
  }
};
