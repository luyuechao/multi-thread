/* one can join() or detach() thread ONLY once
source :
https://www.youtube.com/watch?v=LL8wkskDlbs&list=PL5jc9xFGsL8E12so1wlMS0r0hTQoJL74M

Online information:
https://thispointer.com//c-11-multithreading-part-1-three-different-ways-to-create-threads/
*/

#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

void func1()
{
    cout << "Start sleeping! from thread ID: "
        << this_thread::get_id() << endl;
    chrono::milliseconds timespan(1000);
    for (int i = 3; i>0; i--) {
        cout << i << endl;
        this_thread::sleep_for(timespan);
    }
    cout << "Finish sleeping." << endl;
}

int main()
{
    // use thread in std
    // t1 is construncted using func1()
    // it will start running immediately
    thread t1(func1);
    t1.join(); // main thread waits for t1 to finish

    /*************************/
    thread t2(func1);

    // t2 will run freely on its own which is called daemon process
    // in some system message will not be displayed because the main
    // thread returned before t2 is executed

    t2.detach(); // once detach, forever deatched, cannot join

    if(t2.joinable()) {
        t2.join();
    } else {
        cout << "thread 2 is not joinable."<< endl;
    }

    cout <<"main thread " << this_thread::get_id() << "finished."
        << endl;
    return 0;
}

/*
 Important things to remeber:
(1) If two threads are sharing the same resource,
    e.g. cout for main thread and t1,
    the thread that owns that resource should live (wait for)
    as long as the other thread finish.
(2)
    Thread can be detached ONLY once.
    Thread can not be joined after detach.
    t1.detach();
    t1.join(); // Runtime error, once deatched, forever detached.
    Therefore check joinable() every time for join()
*/