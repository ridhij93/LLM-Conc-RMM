#include <thread>
#include <iostream>
#include <pthread.h>
#include <atomic>
#include <assert.h>
using namespace std;


atomic_int x;
atomic_int y;
atomic_int a;

void *thread1(void *threadid)
{
    // Atomic store operations to set the values of x and y
    atomic_store(&x, 1);
    atomic_store(&y, 1);
}

void *thread2(void *threadid)
{
    int p;
    // Atomic store operation to set the value of y
    atomic_store(&y, 2);
    // Atomic load operation to read the value of x into p
    p = atomic_load(&x);
    // Conditional atomic store operation for a
    if (p == 0)
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
  assert( y != 2  ||   a != 1 );
  // if ((y==2) && (a==1))
  //   cout << "Assertion failed" << endl;
}
