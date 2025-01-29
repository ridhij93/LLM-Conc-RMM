
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

mutex myMutex;

void shared_cout(int i)
{
    lock_guard<mutex> g(myMutex);
    cout << " " << i << " ";
}

void f(int n)
{
    for(int i = 10*(n-1); i < 10*n ; i++) {
        shared_cout(i);
    }
}

int main()
{
    thread t1(f, 1);  
    thread t2(f, 2);  
    thread t3(f, 3);  
    thread t4(f, 4);  
    thread t5(f, 5);  


    for(int i = 0; i > -50; i--)
        shared_cout(i);  

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    return 0;
}
