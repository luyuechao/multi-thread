// argument of thread can only passed by value, not reference
// pass by shared memory will cause data race problem
//
// https://thispointer.com/c11-multithreading-part-3-carefully-pass-arguments-to-threads/

#include <iostream>
#include <thread>
#include <utility>
using namespace std;

// pass by value
void f1(string msg){
    cout << "Thread says: " << msg << endl;
}
// pass by ref
// note that the const, which means we cannot modify 
// thread inside
void f2(const string &msg) {
    cout << "Thread says: " << msg << endl;
    //msg = "Change the string"; //COMPILE ERROR
}

class Fctor {
public:
    void operator()(string& msg) {
        cout << "thread says: " << msg << endl;
    }
};

class MyClass {
public:
	void memberFun( string parm ) {
        cout << parm << endl;
    }
};

int main() 
{
    cout << "*** (1) Passing regular function to thread *************" << endl;
    string arg = "I am an argument.";
    //the syntax is thread(function, paramter).
    thread t1(f1, arg);
    t1.join();

    thread t2(f2, arg);
    t2.join();

    cout << "*** (2) Passing functor to thread **********************" << endl;
    // Method (2): pass string by reference using std::ref
    //thread t2((Fctor()), s);//  Compile Error
    // because paramter always passed by value to a thread
    thread t3((Fctor()), std::ref(arg));

    t3.join();
    cout << "Must use std::ref() to pass arugment to an object with reference parameter"<< endl;
    //string s2 = "Hello";
    //thread t4((Fctor()), std::move(s));
    //t4.join();

    cout << "*** (3) Passing non-static class member function to thread ****" << endl;
    // source : https://thispointer.com/c11-start-thread-by-member-function-with-arguments/
    MyClass obj;
    thread t5(&MyClass::memberFun, &obj, arg);
    
    t5.join();
    return 0;
}
