#include <thread>
#include <iostream>
#include <pthread.h>
#include <atomic>
#include <assert.h>

using namespace std;

atomic_int x;
atomic_int a;

void *thread1(void *threadid)
{
    int p;
    x.store(1, std::memory_order_relaxed);
    p = x.load(std::memory_order_relaxed);
    if (p == 2)
    {
        a.store(1, std::memory_order_relaxed);
    }
}

void *thread2(void *threadid)
{
    x.store(2, std::memory_order_relaxed);
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
  assert( x != 1 || a != 1);
}
