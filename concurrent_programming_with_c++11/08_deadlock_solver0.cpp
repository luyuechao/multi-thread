/*
 * Solution 0: lock ONLY one mutex in one scope
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
    void shared_print(string id, int value) {
        {
            lock_guard<mutex> locker1(mu1);
            f << "From " << id << "thread: " << value << endl;
        }
        {
            lock_guard<mutex> locker2(mu2);
            // the cout is for demo
            cout << "From " << id << ": " << value << endl;
        }
    }
    void shared_print2(string id, int value) {
        {
            lock_guard<mutex> locker2(mu2); 
            cout << "From " << id << ": " << value << endl;
        }
        { 
            lock_guard<mutex> locker1(mu1);
            f << "From " << id << "thread: " << value << endl;
        }
    }

    ~LogFile(){
        f.close();
    }

};

void f1(LogFile& log){
    for (int i = 0; i >-100; i--) {
        log.shared_print(string("From t1: "), i);
    }
}

int main() {

    LogFile obj;
    // pass obj by reference
    // t1 locks mu
    // main locks mu2
    // t1 waits main to unlock mu2
    // main waits t1 to unlock mu
    thread t1(f1, ref(obj));

    for (int i = 0; i < 10; i++){
        obj.shared_print2(string("From main: "), i);
    }

    t1.join();

    cout << "check the log.txt file" << endl;
    return 0;
}
