/*
mutex in a class
https://thispointer.com//c11-multithreading-part-5-using-mutex-to-fix-race-conditions/

check leetcode 1115 print foobar alternately for interleaving exectuion by mutex.

RAII
https://stackoverflow.com/questions/2321511/what-is-meant-by-resource-acquisition-is-initialization-raii
We need to be able to control certain resource. 
The 'Scope-bound' aspect means that the lifetime of the 
object is bound to the scope of a variable, 
so when the variable goes out of scope then the destructor 
will release the resource. 
*/

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

class MyClass
{
private:
    long long var0 = 0;
    long long var_mutex = 0;

    mutex _mu;

public:
    MyClass() {
        cout << "value without mutex : var0 = " << var0 << endl;
        cout << "value with mutex : var_mutex = " << var_mutex << endl;
    }
    void addVal(int t){
        for (int i=0; i<t; ++i){
            var0++;
        }
    }
    // unsafe for exception came after locking the mutex. 
    void addVal_mutex(int t){
        _mu.lock();
        for ( int i=0; i<t; ++i ) {
            var_mutex++;
        }
        _mu.unlock();
    }
    /* lock_guard<mutex> is safter
       std::lock_guard is a class template, which implements 
       the RAII for mutex. It wraps the mutex inside it’s object 
       and locks the attached mutex in its constructor.
       When it’s destructor is called it releases the mutex.
    */
    void addVal_lock_guard(int t){
        lock_guard<mutex> locker( _mu );
        for (int i=0; i < t; ++i){
            var_mutex++;
        }
    }

    void print() {
        cout << "var without mutex  = " << var0 << endl;
        cout << "var_mutex = " << var_mutex << endl;
    }
};

void test1()
{
    MyClass obj;
    int num = 10;
    thread arr[num];
    // create 10 threads share the same obj
    for (int i=0; i<num; ++i){
        arr[i] = thread(&MyClass::addVal, &obj, 10000);
    }
    for (int i=0; i <num; ++i){
        arr[i].join();
    }

    for (int i=0; i<num; ++i){
        arr[i] = thread(&MyClass::addVal_mutex, &obj, 10000);
    }

    for (int i=0; i <num; ++i){
        arr[i].join();
    }
    obj.print();
}

void test2()
{
    MyClass obj;
    int num = 10;
    thread arr[num];
    // create 5 threads share the same obj
    for (int i=0; i<num; ++i){
        arr[i] = thread(&MyClass::addVal_lock_guard, &obj, 10000);
    }
    for (int i=0; i <num; ++i){
        arr[i].join();
    }
    obj.print();
}

int main() {
    cout << "******* lock by mutex lock() and unlock() ***********" << endl;
    for (int i=0; i<10; ++i){
        test1();
    }
    cout << "******** use lock_guard() *****************" << endl;
    test2();
    return 0;
}
