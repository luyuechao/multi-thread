/*
https://levelup.gitconnected.com/producer-consumer-problem-using-mutex-in-c-764865c47483
navie method using a single flag.
both producer thread and consumer thread use unique_lock to 
read/write to critical section.
Use a global flag 
*/

#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
using namespace std;

std::mutex g_mutex;
bool g_ready_flag = false;
int g_data = 0;

int produceData() {
  int n = rand() % 1000;
  std::cout << "produce: " << n << "\n";
  return n;
}

void consumeData(int data) { std::cout << "comsume: " << data << "\n"; }

// consumer thread function
void consumer() {
  while (true) {
    while (!g_ready_flag) { // busy waiting
      // sleep for 1 second
      std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }
    unique_lock<std::mutex> ulock(g_mutex);
    consumeData(g_data);
    g_ready_flag = false;
  }
}

// producer thread function
void producer() {
  while (true) {
    std::unique_lock<std::mutex> uqlock(g_mutex);

    g_data = produceData();
    g_ready_flag = true;
    uqlock.unlock();
    while (g_ready_flag) {
      // sleep for 1 second
      std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }
  }
}

int main() {
  std::thread t1(consumer);
  std::thread t2(producer);
  t1.join();
  t2.join();
  return 0;
}
