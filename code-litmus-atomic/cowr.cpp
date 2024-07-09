#include <thread>
#include <iostream>
#include <pthread.h>
#include <atomic>
#include <assert.h>
// #include <stdatomic.h>

using namespace std;

// Global variables
atomic_int x;
atomic_int a;

void *thread1(void *threadid)
{
    int p;
    // Atomic store operation to set the value of x to 1
    atomic_store(&x, 1);
    // Atomic load operation to read the value of x into p
    p = atomic_load(&x);
    // Conditional atomic store operation for a
    if (p == 2)
    {
        atomic_store(&a, 1);
    }
}

void *thread2(void *threadid)
{
    // Atomic store operation to set the value of x to 2
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
  assert( x != 1 || a != 1);
  // if (x==1 && a==1)
    // std::cout << "Assertion Failed "<< '\n';
}
