/* 
Race condtion (mainly data race) and solve it using mutex
 
 Race conditon :
 A "situation" that may occur inside a critical section. 
 This happens when the result of multiple thread execution 
 in critical section differs according to the order 
 in which the threads execute.

 Critical section:
 A "code segment" where the shared variables can be accessed. 
 An atomic action is required in a critical section i.e. 
 only one process can execute in its critical section at a 
 time. 
 All the other processes have to wait to execute 
 in their critical sections

use mutex to syncrhonize the access to the common resource
*/

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;
const std::string red("\033[0;31m");
const std::string green("\033[0;32m");
const std::string resetColor("\033[0m");

// function with race conditon
void func0() {
    for (int i = 0; i < 100; i++) {
        cout << red << i << resetColor;
    }
}

// (1) create mutex object as global
mutex mu;

// becuase context between lock and unlock may throw exception
// it is unsafe to use lock() and lock()
void func1() {
    mu.lock();
    // critical section
    for (int i=0; i<100; i++) {
        cout << red << i << resetColor;
    }
    // end of critical section
    mu.unlock();
}

// use Resource Acquisition Is Initialization (RAII)
void func2()
{
    // whenever the resource run out of scope
    // the resource will be released
    lock_guard<mutex> guard( mu ); // RAII
    for (int i = 0; i < 100; i++) {
        cout << red << i << resetColor;
    }
}

int main() 
{
    thread t1( func0 );
    for (int i=0; i < 100; i++){
        cout << green << i << resetColor;
    }

    t1.join();
    cout << endl;
    cout << "The output is messy, because t1 and main thread both try to access print" << endl;
    cout << "Race condition for print resource " << endl;

    cout << "/********************************************/" << endl;

    thread t2( func2 );
    mu.lock();
    for (int i = 0; i < 100; i++ ){
        cout << green << i << resetColor;
    }
    mu.unlock();
    t2.join();
    
    cout << endl;
    return 0;
}
