/*
Deadlock is a situation where a set of processes are blocked because each 
process is holding a resource and waiting for another resource acquired by
some other process. 

    thread dead lock 
    two mutex to protect 2 resources 
    they are put in the same scope but diferenct order
    thread 1 locks mu1_
     main thread locks mu2_
    // thread 1 waits main to unlock mu2_
    //  waits t1 to unlock mu1_
    // Then dead look happens

deallock
https://www.geeksforgeeks.org/introduction-of-deadlock-in-operating-system/
*/
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
using namespace std;

// the mutex is bound with resource f
// so never return f to the outside world
class LogFile {
private:
    mutex mu1_;
    mutex mu2_;
    ofstream of;

// note locking mu and mu2_ in 2 methods is NOT the same order
public:
    LogFile() {
        of.open("log.txt");
    } // Need destructor to close file
    void shared_print1(string id, int value) {
        lock_guard<mutex> locker1(mu1_);
        lock_guard<mutex> locker2(mu2_);
        of << "From " << id << "thread: " << value << endl;
        // the cout is for demo
        cout << "From " << id << ": " << value << endl;
    }

    void shared_print2(string id, int value) {
        lock_guard<mutex> locker2(mu2_);
        lock_guard<mutex> locker1(mu1_);
        of << "From " << id << "thread: " << value << endl;
        cout << "From " << id << ": " << value << endl;
    }

    ~LogFile(){
        of.close();
    }
};

void func1(LogFile& log){
    for (int i = 0; i >-100; i--) {
        log.shared_print1("From t1: ", i);
    }
}

int main() {
    LogFile log;
    // pass the log by reference

    thread t1(&func1, std::ref(log));

    for (int i = 0; i < 100; i++){
        log.shared_print2("From main: ", i);
    }

    t1.join();

    cout << "check the log.txt file" << endl;
    return 0;
}
