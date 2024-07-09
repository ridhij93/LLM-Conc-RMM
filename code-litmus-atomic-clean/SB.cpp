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

    // Non-atomic store operation to set x to 1
    atomic_store(&x, 1);

    // Atomic load operation to read y
    p = atomic_load(&y);

    // Check condition atomically
    if (p == 0)
    {
        // Atomic store operation to set a to 1
        atomic_store(&a, 1);
    }

}

void *thread2(void *threadid)
{
    int q;

    // Non-atomic store operation to set y to 1
    atomic_store(&y, 1);

    // Atomic load operation to read x
    q = atomic_load(&x);

    // Check condition atomically
    if (q == 0)
    {
        // Atomic store operation to set b to 1
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
  assert ( a != 1 || b != 1);
  // if ((a==1) && (b==1))
  //   cout << "Assertion failed" << endl;
}
