// argument of thread can only passed by value, not reference
// pass by shared memory will cause data race problem
//
// https://thispointer.com/c11-multithreading-part-3-carefully-pass-arguments-to-threads/

#include <iostream>
#include <thread>
#include <utility>
#include <functional>
using namespace std;

// pass by value
void func1(string msg){
    cout << "Thread says: " << msg << endl;
}
// pass by ref
// note that the const, which means we cannot modify 
// thread inside
void func2(const string& msg) {
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
	void memberFun(string parm) {
        cout << parm << endl;
    }
};

int main()
{
    cout << "*** (1) Regular function *************" << endl;
    string arg = "I am an argument.";
    //the syntax is thread(function, Arg...).
    std::thread t1(func1, arg);
    t1.join();

    std::thread t2(func2, arg);
    t2.join();

    cout << "*** (2) functor **********************" << endl;
    // Method (2): pass string by reference using std::ref
    //thread t2((Fctor()), s);//  Compile Error
    // because paramter always passed by value to a thread
    std::thread t3((Fctor()), std::ref(arg));

    t3.join();
    cout << "Must use std::ref() to pass arugment to an object with reference parameter"<< endl;
    //string s2 = "Hello";
    //thread t4((Fctor()), std::move(s));
    //t4.join();

    cout << "*** (3) non-static class member function ****" << endl;
    // source : https://thispointer.com/c11-start-thread-by-member-function-with-arguments/
    MyClass obj;
    thread t5(&MyClass::memberFun, &obj, arg);

    t5.join();

    cout << "*** (4) lambda function *** " << std::endl; 
    std::thread t6([] {
        std::cout << "Hello from lambda function!" << std::endl;
    });
    t6.join();

    cout << "*** (5) bind " << endl;
    auto bd = std::bind(func2, std::ref(arg));
    thread t7(bd);
    t7.join();
    return 0;
}
