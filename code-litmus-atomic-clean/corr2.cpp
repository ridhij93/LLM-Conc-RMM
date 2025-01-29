#include <thread>
#include <iostream>
#include <pthread.h>
#include <atomic>
#include <assert.h>

using namespace std;

atomic_int x;
atomic_int a;
atomic_int b;


void *thread1(void *threadid)
{
    atomic_store(&x, 1);
}

void *thread2(void *threadid)
{
    atomic_store(&x, 2);
}

void *thread3(void *threadid)
{
    int p, q;
    p = atomic_load(&x);
    q = atomic_load(&x);
    if (p == 1 && q == 2)
    {
        atomic_store(&a, 1);
    }
    if (p == 2 && q == 1)
    {
        atomic_store(&a, 2);
    }
}

void *thread4(void *threadid)
{
    int r, s;
    r = atomic_load(&x);
    s = atomic_load(&x);
    if (r == 2 && s == 1)
    {
        atomic_store(&b, 1);
    }
    if (r == 1 && s == 2)
    {
        atomic_store(&b, 2);
    }
}


int main()
{
  int i=0;
  int j=1;
  int rc1,rc2, rc3, rc4;
  pthread_t threads[4];
  rc1 = pthread_create(&threads[0], NULL,
                          thread1, (void *)i);
  rc2 = pthread_create(&threads[1], NULL, 
                          thread2, (void *)j);
  rc3 = pthread_create(&threads[2], NULL,
                          thread3, (void *)i);
  rc4 = pthread_create(&threads[3], NULL, 
                          thread4, (void *)j);
  (void) pthread_join(threads[0], NULL);
  (void) pthread_join(threads[1], NULL);
  (void) pthread_join(threads[2], NULL);
  (void) pthread_join(threads[3], NULL);
  assert ( ( a != 1 || b != 1) && (a != 2 || b != 2));
}
