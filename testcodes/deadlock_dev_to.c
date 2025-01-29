#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex cout_key;

void my_work() {
    cout_key.lock();
    std::cout << "Thread" << '[' << std::this_thread::get_id() << ']' << " acquired the lock" << '\n';
    
}

int main() {
    std::vector<std::thread> my_threads;
    for(int i=0; i<2; i++) {
        my_threads.emplace_back(my_work);
    }
    for(auto& i: my_threads) {
        i.join();
    }
    return 0;
}
