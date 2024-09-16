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
    atomic_store(&x, 2);
    atomic_store(&y, 1);
}

void *thread2(void *threadid)
{
    atomic_store(&y, 2);
    atomic_store(&x, 1);
}

void *thread3(void *threadid)
{
    int p = atomic_load(&x); 
    int q = atomic_load(&x); 
    if (p == 1 && q == 2)
        atomic_store(&a, 1);
}

void *thread4(void *threadid)
{
    int p = atomic_load(&y); 
    int q = atomic_load(&y); 
    if (p == 1 && q == 2)
        atomic_store(&b, 1);
}


int main()
{
  int i=0;
  int rc1,rc2, rc3, rc4;
  pthread_t threads[4];
  rc1 = pthread_create(&threads[0], NULL,
                          thread1, (void *)i);
  rc2 = pthread_create(&threads[1], NULL, 
                          thread2, (void *)i);
  rc3 = pthread_create(&threads[2], NULL,
                          thread3, (void *)i);
  rc4 = pthread_create(&threads[3], NULL, 
                          thread4, (void *)i);
  (void) pthread_join(threads[0], NULL);
  (void) pthread_join(threads[1], NULL);
  (void) pthread_join(threads[2], NULL);
  (void) pthread_join(threads[3], NULL);
  assert (a != 1 || b != 1);
}
