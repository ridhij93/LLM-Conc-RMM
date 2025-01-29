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
    atomic_store(&x, 1);
    std::atomic_thread_fence(std::memory_order_seq_cst);
    p = atomic_load(&y);
    if (p == 0)
    {
        atomic_store(&a, 1);
    }

}

void *thread2(void *threadid)
{
    int q;

    atomic_store(&y, 1);

    q = atomic_load(&x);

    if (q == 0)
    {
        atomic_store(&b, 1);
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
