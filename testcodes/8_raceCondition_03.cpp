#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>


int sharedVariable_race_01 = 0;

void increment_race_01() {
    for (int i = 0; i < 100000; ++i) {
        sharedVariable_race_01++;
    }
}


int sharedVariable_race_02 = 0;
int currentValue_02 = sharedVariable_race_02;

void increment_race_02() {
    for (int i = 0; i < 100000; ++i) {
        int currentValue_02 = sharedVariable_race_02;
        std::this_thread::sleep_for(std::chrono::microseconds(1)); 
        sharedVariable_race_02 = currentValue_02 + 1;
    }
}



int sharedVariable_mutex_01 = 0;
std::mutex mtx_1; 
int currentValue_mutex = sharedVariable_mutex_01;

void increment_mutex_01() {
    for (int i = 0; i < 100000; ++i) {
        mtx_1.lock(); 
        int currentValue_mutex = sharedVariable_mutex_01;
        std::this_thread::sleep_for(std::chrono::microseconds(1)); 
        sharedVariable_mutex_01 = currentValue_mutex + 1;
        mtx_1.unlock(); 
    }
}



int sharedVariable_mutex_02 = 0;
std::mutex mtx_2; 

void increment_mutex_02() {
    for (int i = 0; i < 100000; ++i) {
        std::lock_guard<std::mutex> lock(mtx_2); 
        sharedVariable_mutex_02++;
    }
}




std::atomic<int> sharedVariable_atomic(0);

void increment_atomic() {
    for (int i = 0; i < 100000; ++i) {
        sharedVariable_atomic++;
    }
}



int main() {
    std::thread thread1(increment_race_01);
    std::thread thread2(increment_race_01);
    std::thread thread3(increment_race_02);
    std::thread thread4(increment_race_02);

    std::thread thread5(increment_mutex_01);
    std::thread thread6(increment_mutex_01);
    std::thread thread7(increment_mutex_02);
    std::thread thread8(increment_mutex_02);

    std::thread thread9(increment_atomic);
    std::thread thread10(increment_atomic);

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    thread6.join();
    thread7.join();
    thread8.join();
    thread9.join();
    thread10.join();

    std::cout << "Expected value: 200000" << std::endl;

    std::cout << "Actual value of Race Condition 01: " << sharedVariable_race_01 << std::endl;
    std::cout << "Actual value of Race Condition 02: " << sharedVariable_race_02 << std::endl;

    std::cout << "Actual value of Mutex 01: " << sharedVariable_mutex_01 << std::endl;
    std::cout << "Actual value of Mutex 02: " << sharedVariable_mutex_02 << std::endl;

    std::cout << "Actual value of Atomic: " << sharedVariable_atomic.load() << std::endl;

    return 0;
}

