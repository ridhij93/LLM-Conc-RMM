#include <iostream>
#include <atomic>
#include <thread>
#include <assert.h>
#include <pthread.h>

std::atomic<int> x(0);
std::atomic<int> y(0);
int value1, value2;
void *Thread0(void * threadid) {
    value1 = x.load(std::memory_order_relaxed);
    y.store(1, std::memory_order_relaxed);
    // flag.store(1, std::memory_order_release);
}

void *Thread1(void * threadid) {
    value2 = y.load(std::memory_order_relaxed);
    x.store(1, std::memory_order_relaxed);
}

int main() {
  int i=0;
  int j=1;
  int rc1,rc2;
  pthread_t threads[2];
  rc1 = pthread_create(&threads[0], NULL,
                          Thread0, (void *)i);
  rc2 = pthread_create(&threads[1], NULL, 
                          Thread1, (void *)j);
  (void) pthread_join(threads[0], NULL);
  (void) pthread_join(threads[1], NULL);
    assert( value2 == 0 || value1 == 0);
    return 0;
}
