// one can only join() or detach() thread only once
// one must decide thread to join or detach
// source :
// https://www.youtube.com/watch?v=LL8wkskDlbs&list=PL5jc9xFGsL8E12so1wlMS0r0hTQoJL74M

#include <iostream>
#include <thread>
using namespace std;

void func1() {
    std::cout << "Hello, world!" << endl;
}

class myFunctor {
public:
    void operator()() { // first () means paranthesis is overloaded, 
                        // second means no argument for this method
        for (int i = 0; i >-10; i--) {
            cout << "From t1 thread: " << i << endl;
        }
    }
};

int main() {
    myFunctor fct;
    thread t1(fct);
    // main thread do some work
    try {
        for (int i = 0; i < 10; i++ ) {
            cout << "From main thread: " << i << endl;
        }
    } catch (...) { // any exception
        t1.join();
        throw;
    }

    cout <<"!!!Both threads are racing for the IO common resource, which is called race condition!!!" << endl;

    return 0;
}
