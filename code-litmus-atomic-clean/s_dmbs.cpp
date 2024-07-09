#include <thread>
#include <iostream>
#include <pthread.h>
#include <atomic>
#include <assert.h>

using namespace std;
std::atomic<int> x;
std::atomic<int> y;
std::atomic<int> a;

void *thread1(void *threadid)
{
    // Non-atomic store operation to set x to 2
    atomic_store(&x, 2);

    // Memory barrier (full fence)
    std::atomic_thread_fence(std::memory_order_seq_cst);

    // Non-atomic store operation to set y to 1
    atomic_store(&y, 1);

}

void *thread2(void *threadid)
{
    int p;

    // Non-atomic load operation to read y
    p = atomic_load(&y);

    // Memory barrier (full fence)
    std::atomic_thread_fence(std::memory_order_seq_cst);

    // Non-atomic store operation to set x to 1
    atomic_store(&x, 1);

    // Check condition atomically
    if (p == 1)
    {
        // Non-atomic store operation to set a to 1
        atomic_store(&a, 1);
    }

}


int main()
{
  int i=0;
  int j=1;
  int rc1,rc2;
  pthread_t threads[2];
  rc1 = pthread_create(&threads[0], NULL,
                          thread1, (void *)i);
  rc2 = pthread_create(&threads[1], NULL, 
                          thread2, (void *)j);
  (void) pthread_join(threads[0], NULL);
  (void) pthread_join(threads[1], NULL);
  assert(x != 2 || a != 1);
  // if ((x==2) && (a==1))
  //   cout << "Assertion failed" << endl;
}
