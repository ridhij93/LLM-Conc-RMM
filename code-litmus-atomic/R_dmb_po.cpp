
#include <thread>
#include <iostream>
#include <pthread.h>
#include <atomic>
#include <assert.h>

using namespace std;

// Global variables
std::atomic<int> x;
std::atomic<int> y;
std::atomic<int> a;

void *thread1(void * threadid)
{
    atomic_store(&x, 1);
    std::atomic_thread_fence(std::memory_order_seq_cst); // Memory barrier
    atomic_store(&y, 1);
}

void *thread2(void * threadid)
{
    atomic_store(&y, 2); // Atomic store operation to set y to 2
    // int p = x.load(std::memory_order_relaxed); // Atomic load operation to read x into p
    int p = atomic_load(&x);
    if (p == 0)
    {
        atomic_store(&a, 1);// Atomic store operation to set a to 1
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
  assert( y != 2 || a != 1);
}
