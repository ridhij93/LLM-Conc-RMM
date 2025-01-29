#ifndef USE_STD_LOCK
#   error #define USE_STD_LOCK as 1 to use std::lock and as 0 to use ordering
#endif

#include <atomic>
#include <chrono>
#include <exception>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

std::thread task[5];
constexpr auto N = sizeof(task)/sizeof(task[0]);
std::mutex chopstick[N];
std::atomic<bool> stop{false};
unsigned long long counts[N] = {};

using namespace std::chrono_literals;

void
go(decltype(N) i)
{
    auto const right = (i + 1) % N;
    decltype(right) const left = i;
    while (!stop)
    {
#if USE_STD_LOCK
        std::lock(chopstick[left], chopstick[right]);
#else
        if (left < right)
        {
            chopstick[left].lock();
            chopstick[right].lock();
        }
        else
        {
            chopstick[right].lock();
            chopstick[left].lock();
        }
#endif
        std::lock_guard<std::mutex> l1{chopstick[left],  std::adopt_lock};
        std::lock_guard<std::mutex> l2{chopstick[right], std::adopt_lock};
        ++counts[i];
        std::this_thread::sleep_for(1ms);
    }
}

void
deadlock_detector(std::chrono::seconds time_out)
{
    std::this_thread::sleep_for(time_out);
    std::cerr << "Deadlock!\n";
    std::terminate();
}

int
main()
{
    for (auto i = 0u; i < N; ++i)
        task[i] = std::thread{go, i};
    std::thread{deadlock_detector, 15s}.detach();
    std::this_thread::sleep_for(10s);
    stop = true;
    for (auto& t : task)
        t.join();
    std::cout << std::right;
    for (auto c : counts)
        std::cout << std::setw(6) << c << '\n';
    auto count = std::accumulate(std::begin(counts), std::end(counts), 0ULL);
    std::cout << "+ ----\n";
    std::cout << std::setw(6) << count << '\n';
}

