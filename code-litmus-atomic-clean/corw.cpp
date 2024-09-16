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
    p = atomic_load(&x);
    atomic_store(&x, 1);
    if (p == 2)
    {
        atomic_store(&a, 1);
    }
}
void *thread2(void *threadid) {
    atomic_store(&x, 2);
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
  assert (a!=1 || x!=2);
}
