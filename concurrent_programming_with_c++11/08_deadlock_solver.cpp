/*
 * Solution 1: all threads locks mutex in the same order.
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
    ofstream f;

public:
    LogFile() {
        f.open("log.txt");
    } // Need destructor to close file
    void shared_print1(string id, int value) {
        lock_guard<mutex> locker1(mu1);
        lock_guard<mutex> locker2(mu2);
        f << "From " << id << "thread: " << value << endl;
        // the cout is for demo
        cout << "From " << id << ": " << value << endl;
    }
    void shared_print2(string id, int value) {
        lock_guard<mutex> locker1(mu1);
        lock_guard<mutex> locker2(mu2);
        f << "From " << id << "thread: " << value << endl;
        cout << "From " << id << ": " << value << endl;
    }

    ~LogFile(){
        f.close();
    }

};

void func1(LogFile& log){
    for (int i = 0; i >-100; i--) {
        log.shared_print1(string("From t1: "), i);
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

    for (int i = 0; i < 10; i++){
        log.shared_print2(string("From main: "), i);
    }

    t1.join();

    cout << "check the log.txt file" << endl;
    return 0;
}
