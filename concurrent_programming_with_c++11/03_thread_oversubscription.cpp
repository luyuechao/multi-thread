/*
creating too many threads than hardware can handle
will cause oversubscription
which leads to too many context swithing and degrade the
performance
*/
#include <iostream>
#include <thread>
using namespace std;

int main() {
    // Oversubscription
    // gives me how many threads I can really run concurrently.
    cout << "I can run " << std::thread::hardware_concurrency()
        << " threads concurrently on this system." << endl;

    return 0;
}
