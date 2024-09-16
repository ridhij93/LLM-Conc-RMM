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
    int p;
    x.store(1, std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_seq_cst);
    p = y.load(std::memory_order_relaxed); 
    if (p == 0)
    {
        a.store(1, std::memory_order_relaxed);
    }

}

void *thread2(void *threadid)
{
    int q;

    y.store(1, std::memory_order_relaxed);

    q = x.load(std::memory_order_relaxed); 

    if (q == 0)
    {
        b.store(1, std::memory_order_relaxed);
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
  assert ( a != 1 || b != 1 );
}
