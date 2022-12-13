/*
What is concurrency?
program runs at the same time
(1) context-swithing task-swithcing
(2) hardware concurrency

Two concurrent programming model
Process vs. Threads
(1) multiprocessing - distributed memory programming
  (a) Processes communicate with each other using interprocess 
      communication channel (message passing)
      e.g. files, pipe (> cat a.txt | grep hello) socket (> curl http://...)
  (b) instance of a computer program that is being executed
  (c) A process can have one or multiple threads
  (d) create a parallel universe by pid_t pid = pork(),
  everything will be duplicated including registers, variable, PC, SP)
    (shared libaray will still be shared through virtual memory)

(2) multithreading - shared memory programming
   a. smallest sequence of programmed instructions that can be 
      managed independently by a scheduler
   b. thread has its own registers e.g. PC (program counter), SP(stack pointer)
   c. One process can has two or more threads.
   d. Threads communicate with each other through shared memory
Pros:
  * Fast to starts
  * Low overhead
Cons:
  * difficult to implement
  * can't run on distributed system

  Online information:
  https://thispointer.com//c-11-multithreading-part-1-three-different-ways-to-create-threads/

 4 ways to create a thread
 (1) function pointer
 (2) function of an object
 (3) functor
 (4) lambda function

source :
https://blog.csdn.net/fuxuemingzhu/article/details/95889253

Does std::thread library in C++ support nested threading?
https://stackoverflow.com/questions/42806828/does-stdthread-library-in-c-support-nested-threading

*/

#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

void func() 
{
    cout << "Start sleeping! from thread ID: " 
        << this_thread::get_id() << endl;
    chrono::milliseconds timespan(100);
    for (int i = 3; i>0; i--) {
        cout << i << "  ";
        this_thread::sleep_for(timespan);
    }
    cout << "Finish sleeping." << endl;
}

class MyClass
{
public:
    void operator()()
    {
        func();
    }
};

int main() {
    thread t0; // T0 is not a thread!
    cout << "******** 4 ways to create thread ***********" << endl;
    // thread constructor
    // https://en.cppreference.com/w/cpp/thread/thread/thread
    cout << " **** (1) passing the function pointer." << endl;
    // it will start running immediately
    thread t1( func );
    // Note that without &, it still works
    t1.join(); // tell the main thread to wait t1

    cout <<  " **** (2.0) passing an object with overloaded () method " << endl;

    MyClass obj;
    thread t2( obj );
    t2.join();
    
    cout << " **** (2.1) Creating a threading using functor." << endl;
    /*
        thread t3( MyClass() ); // Compile ERROR
    */
    thread t3( (MyClass()) ); // note the double ()
    /* the double () tells the compiler, this is a functor
     for a detailed explaination see
     https://www.youtube.com/watch?v=f2nMqNj7vxE&list=PL5jc9xFGsL8E12so1wlMS0r0hTQoJL74M&index=2
    */
    t3.join();

    cout << " **** (3) Creating a thread using lambda function." << endl;
    chrono::milliseconds timespan(100);
    auto lda = [](){ for (int i = 3; i>0; i--)  cout << i << endl; };
    thread t4(lda);
    t4.join();

}
