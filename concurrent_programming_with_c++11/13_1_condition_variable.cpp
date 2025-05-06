/*

https://www.youtube.com/watch?v=13dFggo4t_I&list=PL5jc9xFGsL8E12so1wlMS0r0hTQoJL74M&index=6

condition variable: remove the busy waiting between threads
or syncrhonize the exectuion sequence of threads

we must use unique_lock for condition variable
we can NOT use lock_guard

usage: thread pool

Use notify_one() for single thread

effective modern c++ item 39
Note oneshot communcation always future & promise
*/

#include <deque>
#include <functional>
#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <condition_variable>
using namespace std;

std::deque<int> gDeq;
std::mutex mu;
std::condition_variable g_cv;

/*
two threads: 1 producer and 1 cosumer
one resource in the q
global mutex mu to protect the access
*/

// producer pushes data on global q
void producer() {
	int count = 10;
	while (count > 0) {
		unique_lock<mutex> locker(mu);
		gDeq.push_front(count);
		locker.unlock();
		this_thread::sleep_for(chrono::seconds(1));
		count--;
	}
}

// consumer check q, and pop the data until data == 1
void consumer() {
	int data = 0;
	// busy waiting state for consumer check emptyness q 
	while ( data != 1) {
		unique_lock<mutex> locker(mu);
		if (!gDeq.empty()) {
			data = gDeq.back();
			gDeq.pop_back();
			locker.unlock();
			cout << "t2 got a value from t1: " << data << endl;
		} else {
			locker.unlock();
		}
        // without the sleep, t2 will check q repeatedly, which locks 
        // and unlock mu repeatedly, this is busy waiting
        // busy waiting is very inefficient
        // sleep will reduce the number of checking,
        // but we don't know how to set the sleep time
		this_thread::sleep_for(chrono::milliseconds(5));// inefficient
	}
}

void test0(){
	thread t1(producer);
	thread t2(consumer);
	t1.join();
	t2.join();
}

void producer2() {
	int count = 10;
	while (count > 0) {
		unique_lock<mutex> locker(mu);
		gDeq.push_front(count);
		locker.unlock();
		g_cv.notify_one(); // Notify one waiting thread, if there is one.
		// still need to sleep, for notifying others
		this_thread::sleep_for(chrono::seconds(1));
		count--;
	}
}

void consumer2() {
	int data = 1;
	while ( data != 1) {
		unique_lock<mutex> locker(mu);
    	// cond.wait will put current thread into sleep until get notfiied
		// so it requires the locker as argument
		// g_cv will unlock it before sleep and lock it after wakeup.
		g_cv.wait(locker);
    	data = gDeq.back();
		gDeq.pop_back();
		cout << "t2 got a value from t1: " << data << endl;
		locker.unlock();
	}
}

void consumer3() {
	int data = 1;
	while ( data != 1) {
		unique_lock<mutex> locker(mu);
		// spurious wake, takes lambda function as argument
		g_cv.wait(locker, [](){ return !gDeq.empty(); });
    	data = gDeq.back();
		gDeq.pop_back();
		cout << "t2 got a value from t1: " << data << endl;
		locker.unlock();
	}
}

void test1() {
    thread t1(producer2);
    thread t2(consumer3);
    t2.join();    
    cout << "Note that t1 should join() after t2.join(), or t2 won't get notified" << endl;
    t1.join();
}

int main() {
    cout << "**************** test0() ****************" << endl;
    test0();
    cout << "**************** test1() ****************" << endl;
    test1();

    cout << "threads shall never go to sleep while holding a mutex." << endl;

	return 0;
}
