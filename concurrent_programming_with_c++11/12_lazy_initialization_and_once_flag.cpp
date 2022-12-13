/*
https://www.youtube.com/watch?v=IBu5ka1MQ7w&list=PL5jc9xFGsL8E12so1wlMS0r0hTQoJL74M&index=5
 lazy Initialziation : initialization upon first use idiom
 Use call_once() to initilize resource once
 
 Lazy initialization is the tactic of 
 deferring the creation of an object, memory allocation, 
 the calculation of a value, or some other expensive process 
 until the first time it is needed. 
 It is a kind of lazy evaluation that refers specifically to 
 the instantiation of objects or other resources.
 This is typically accomplished by augmenting an accessor method 
 (or property getter) to check whether a private member, acting as 
 a cache, has already been initialized. If it has, it is returned 
 straight away. If not, a new instance is created, placed into the 
 member variable,  and returned to the caller just-in-time for 
 its first use.
 
 */
#include<iostream>
#include<fstream>
#include<mutex>

using namespace std;


/* we want to open the log.txt ONLY when shared print is called 
then we move ofs.open() into shared_print function.
THis is called:
lazy Initialziation or initialization upon first use idiom
*/

class LogFile {
private:
	std::mutex mtx_; // for writing
    // second mutex is only used to protect opening resources
    std::mutex mtx2_;
    std::once_flag _1flag;
	ofstream ofs;
public:
	LogFile() {
        // ofs may never used if we open it here.
		//ofs.open("log.txt");
	}

    // thread UNsafe, because it locks ofs.open instead of
    // locking the ofs.is_open(), the file may be opened multiple
    // times by different thread
	void shared_print_UNsafe(string id, int value) {
        if (!ofs.is_open()) { // this line is thread unsafe
            unique_lock<mutex> uqlock(mtx2_);
            ofs.open("./log.txt");
        }
		std::unique_lock<mutex> locker(mtx_);
		ofs << "From " << id << ": " << value << endl;
        locker.unlock();
    }
    // thread safe, but consume a lot of resources 
    // every thread will lock mutex, then check the ofs is 
    // open or not
    void share_print1(string id, int value){
        {   // protect both operation with mutex
            unique_lock<mutex> uqlock(mtx2_);
            if (!ofs.is_open()) {
                ofs.open("./log.txt");
            }
        }
		std::unique_lock<mutex> locker(mtx_, std::defer_lock);
		ofs << "From " << id << ": " << value << endl;
        locker.unlock();
    }
    // BEST soluiton
    void share_print2(string id, int value){
        // 
        std::call_once(_1flag, [&](){ofs.open("./log.txt");});

		std::unique_lock<mutex> locker(mtx_);
		ofs << "From " << id << ": " << value << endl;
        locker.unlock();
    }
};

int main(){
    LogFile lf;
    lf.share_print1("dd", 4);
    std::cout << "** use once" << std::endl;
    //lf.share_print2("ss", 3);
}
