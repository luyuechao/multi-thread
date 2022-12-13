/* 
   (1) Use mutex to syncrhonize the access to the common resource
   (2) Never leak a handle of data to outside
   (3) Design interface appropriately
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
    mutex _mu;
    ofstream _of;

public:
    LogFile() {
        _of.open("log.txt");
    }
    void shared_print(string id, int value) {
        lock_guard<mutex> locker( _mu );
        _of << "From " << id << "thread: " << value << endl;
    }
    ~LogFile(){
        _of.close();
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
    thread t1(&func1, ref(log));

    for (int i = 0; i < 100; i++){
        log.shared_print(string("From main: "), i);
    }

    t1.join();

    cout << "check the log.txt file" << endl;
    return 0;
}
