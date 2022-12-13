/*
 notify multiple threads
*/
#include <deque>
#include <functional>
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <mutex>
#include <condition_variable>
#include <climits>
using namespace std;

deque<int> q;
mutex mu;
condition_variable g_cond;
// 
void producer() {
	int count = 10;
	while (count > 0) {
		unique_lock<mutex> locker(mu);
		q.push_front(count);
		locker.unlock();
		// notify one 
		//cond.notify_one(); // Notify one waiting thread, 
		// if there is one.
		g_cond.notify_all();
		// still need to sleep, for notifying others
    this_thread::sleep_for(chrono::seconds(1));
		count--;
	}
}

void consumer() {
	int data = INT_MAX;
	while ( data != 0) {
		unique_lock<mutex> locker(mu);
	  // condition_variable.wait will put current thread into 
		// sleep until get notfiied
		// before this thread is put into sleep, wait() function
		// will unlocker, then go to sleep
		// when it's notified by notify function, it will go back
		// from sleep and lock the locker again.
		// since we need to lock and unlock repeatedly, 
		// we need to use unqiue_lock instead of lock_guard
		g_cond.wait(locker);
    data = q.back();
		q.pop_back();
		cout << "Thread " << this_thread::get_id() << " got a data from producer: [" << data << "]" << endl;
		locker.unlock();
	}
}

void test() {

  thread t1(producer);
	thread t2(consumer);
	thread t3(consumer);
	thread t4(consumer);
	t2.join();
	t3.join();
	t4.join();
	cout << "Note that t1 should join() after all threads" << endl;
    t1.join();
}

int main() {
    cout << "**************** test() ****************" << endl;
    test();


	return 0;
}
