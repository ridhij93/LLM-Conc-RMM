#include <iostream>
#include <thread>
#include <chrono>

void function1() {
  for (int i = 0; i < 200; ++i) {
    std::cout << "A";
    std::this_thread::sleep_for(std::chrono::nanoseconds(1)); 
  }
}

void function2() {
  for (int i = 0; i < 200; ++i) {
    std::cout << "B";
    std::this_thread::sleep_for(std::chrono::nanoseconds(1)); 
  }
}

void function3(char c) {
  for (int i = 0; i < 200; ++i) {
    std::cout << c;
    std::this_thread::sleep_for(std::chrono::nanoseconds(1)); 
  }
}

int main() {
  std::thread worker1(function1);
  std::thread worker2(function2);
  std::thread worker3(function3, '-');
  worker1.join();
  worker2.join();
  worker3.join();
  std::cout << "\n";
}

