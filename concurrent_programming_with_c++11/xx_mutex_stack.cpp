// use stack to pass mutex object
// unfinished program

#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>

using namespace std;

class Mu_stack{
private: 
    int * _data;
    mutex _mu;
public:
    int& pop(); // pops off the item on top of the stack
    //...

};

void func1(stack& st){
    int v = st.pop();
    process(v);
}

int main() {
    
    return 0;
}
