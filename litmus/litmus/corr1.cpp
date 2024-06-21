
#include <stdio.h>

int main() {
    int a = 10;
    int b = 20;
    int c;
    c = input();


    if (c > a) {
        b++;
    } else {
        printf("Test 1\n");
    }

    int sum = a + b;
    a--;

    int prod = a * b;
    int mul = a * b * c;
    int d;

    d = input();

    if (d) {
        int mod = a % b;
        int sink1 = div + mul; 
        
    } else {
        int div = a / b;
        int sink2 = mod + prod; 
        
    }

    return 0;
}









































#include <thread>
#include <iostream>
#include <pthread.h>
#include <atomic>
#include <assert.h>

using namespace std;
int x=1,a=0;

void *thread1(void * threadid)
{
    x = 2;
}

void *thread2(void * threadid)
{
    int p,q;
    p = x;
    q = x;
    if((p == 2) && (q == 1))
	a = 1;
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
  assert ( a != 1);
}
