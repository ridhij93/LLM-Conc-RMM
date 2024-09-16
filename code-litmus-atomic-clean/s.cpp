#include <thread>
#include <iostream>
#include <pthread.h>
#include <assert.h>
#include <atomic>

using namespace std;


std::atomic<int> x;
std::atomic<int> y;
std::atomic<int> a;



void *thread1(void *threadid)
{
    atomic_store(&x, 2);

    atomic_store(&y, 1);

}

void *thread2(void *threadid)
{
    int p;

    p = atomic_load(&y);

    atomic_store(&x, 1);

    if (p == 1)
    {
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
  assert (x != 2 || a != 1);
}
