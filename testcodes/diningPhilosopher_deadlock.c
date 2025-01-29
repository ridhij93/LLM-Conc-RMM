#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <time.h>

using namespace std;

thread task[5];
mutex chopstick[5];
int stop = false;

void go(int i) {
    while (!stop) {

        chopstick[i].lock();

        cout << i << ": takes: " << i << endl;
        chrono::milliseconds dur(20);
        this_thread::sleep_for(dur); 
        chopstick[(i + 1) % 5].lock();

        cout << i << ": eating" << endl;

        chrono::milliseconds dur2(rand() % 200 + 100);
        this_thread::sleep_for(dur2);

        chopstick[(i + 1) % 5].unlock();
        chopstick[i].unlock();
    }
}
int main() {
    srand(time(NULL));

    for (int i = 0; i < 5; ++i) {
        task[i] = (thread(go, i));
    }
    for (int i = 0; i < 5; i++) {
        task[i].join();
    }

}

