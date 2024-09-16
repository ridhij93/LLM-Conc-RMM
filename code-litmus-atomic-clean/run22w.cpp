#include <thread>
#include <iostream>
#include <pthread.h>
#include <atomic>
#include <assert.h>

using namespace std;

std::atomic<int> x;
std::atomic<int> y;

void *thread1(void *threadid)
{
  atomic_store(&x, 2);
  atomic_store(&y, 1);
}

void *thread2(void *threadid)
{
  atomic_store(&y, 3);
  atomic_store(&x, 4);
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

  assert( x != 2 || y != 3 );

}



