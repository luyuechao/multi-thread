/*
 * Solution 1: all threads locks mutex in the same order.
 * Solution 2: use std:lock() to lock 2 mutex

 Conclusion:

 1. Prefer locking a single mutex
 2. Avoid locking a mutex and then calling a user provdied function
 3. Use std::lock() to lock more than one mutex.
 4. lock the mutex in the same order.
 *
 * */
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>

using namespace std;
// the mutex is bound with resource f
// so never return f to the outside world
class LogFile {
private:
    mutex mu1;
    mutex mu2;
    ofstream of;

public:
    LogFile() {
        of.open("log.txt");
    }

    void shared_print(string id, int value) {
        lock(mu1, mu2);
        // adopt_lock tells the locker that
        // the ownership has been locked
        lock_guard<mutex> locker(mu1, adopt_lock);
        lock_guard<mutex> locker2(mu2, adopt_lock);
        of << "From " << id << "thread: " << value << endl;
        cout << "From " << id << ": " << value << endl;
    }
    void shared_print2(string id, int value) {
        lock(mu1, mu2);
        lock_guard<mutex> locker2(mu2, adopt_lock);
        lock_guard<mutex> locker(mu1, adopt_lock);
        of << "From " << id << "thread: " << value << endl;
        cout << "From " << id << ": " << value << endl;
    }

    ~LogFile(){
        of.close();
    }

};

void func1(LogFile& log){
    for (int i = 0; i >-100; i--) {
        log.shared_print(string("From t1: "), i);
    }
}

int main() {

    LogFile log;
    // pass the log by reference
    // t1 locks mu
    // main locks mu2
    // t1 waits main to unlock mu2
    // main waits t1 to unlock mu
    thread t1(func1, ref(log));

    for (int i = 0; i < 100; i++){
        log.shared_print2(string("From main: "), i);
    }

    t1.join();

    cout << "check the log.txt file" << endl;
    return 0;
}
