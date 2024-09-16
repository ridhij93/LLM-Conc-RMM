#include <thread>
#include <iostream>
#include <pthread.h>
#include <atomic>
#include <assert.h>

using namespace std;
std::atomic<int> x;
std::atomic<int> y;
std::atomic<int> a;
std::atomic<int> b;


void *thread1(void *threadid)
{
    x.store(2, std::memory_order_relaxed);
}

void *thread2(void *threadid)
{
    int p;

    p = x.load(std::memory_order_relaxed);

    y.store(1, std::memory_order_relaxed);

    if (p == 2)
    {
        a.store(1, std::memory_order_relaxed);
    }

}

void *thread3(void *threadid)
{
    y.store(2, std::memory_order_relaxed);
    x.store(1, std::memory_order_relaxed);

}


int main()
{
  int i=0;
  int j=1;
  int rc1,rc2,rc3;
  pthread_t threads[3];
  rc1 = pthread_create(&threads[0], NULL,
                          thread1, (void *)i);
  rc2 = pthread_create(&threads[1], NULL, 
                          thread2, (void *)j);
  rc3 = pthread_create(&threads[2], NULL, 
                          thread3, (void *)j);
  (void) pthread_join(threads[0], NULL);
  (void) pthread_join(threads[1], NULL);
  (void) pthread_join(threads[2], NULL);
  assert( x != 2 || y != 2 || a != 1);
}
