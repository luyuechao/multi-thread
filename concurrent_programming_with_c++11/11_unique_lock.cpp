/*
  unique_lock provides more flexibility to a finer-grained 
  lock.
  Pros:
  (1) lock and unlock anytime we want
  We can lock and unlock it when necessary without any limit
  while lock_guard will lock instantly and cannot be unlock
  before out of scope.
  
  (2)  unique_lock for transferring mutex ownership 
   unique_lock can be moved, while lock_guard can NEVER 
  be moved.
  unique_lock<mutex> uqlock1(mtx_);
  //  unique_lock<mutex> uqlock2 = uqlock1; // Compile error
  unique_lock<mutex> uqlock2 = std::move(uqlock1);
  cons:
  unique_lock is a bit heavier than lock_guard,
  just use the lock_guard for performance

https://www.youtube.com/watch?v=IBu5ka1MQ7w

*/

#include<iostream>
#include<fstream>
#include<mutex>

using namespace std;

class LogFile
{
private:
	std::mutex mtx_;
	ofstream file;
public:
	LogFile() {
		file.open("log.txt");
	}
	unique_lock<mutex> giveMeLock() {
		return unique_lock<mutex>(mtx_);  // Moved
	}

    // use mutex to guard the file output resource
	void shared_print1(string id, int value) {
        // lock_guard<mutex> locker(mtx);
		unique_lock<mutex> uqlock(mtx_);
		file << "From " << id << ": " << value << endl;
        // after accessing file, we can only unlock by using 
        // unique_lock, therefore another threads can access file
        uqlock.unlock();
        cout << "do something else" << endl;
    }

    // declare the locker and lock it later
    void shared_print2(string id, int value) {
		// defer_lock tells locker do not lock the mutex now.
        std::unique_lock<mutex> locker(mtx_, std::defer_lock);

        cout << "do something " << endl;
        locker.lock(); 
		file << "From " << id << ": " << value << endl;
        // after accessing file, we can only unlock by using unique_lock
        // therefore other threads can access file
        locker.unlock();
        cout << "do something, you can lock() and unlock() unique_lock repeatedly,"<< endl;
        cout <<"while lock_guard can NOT." << endl;
    }
    // 
    void shared_print3(string id, int value) {
        // lock_guard<mutex> locker(mtx);
		std::unique_lock<mutex> locker(mtx_);

        cout << "do something " << endl;
        locker.lock();
		file << "From " << id << ": " << value << endl;
        // after accessing file, we can only unlock by using unique_lock
        // therefore another threads can access file
        locker.unlock();
        cout << "do something " << endl;
    }
};

int main(){
    LogFile lf;

}
