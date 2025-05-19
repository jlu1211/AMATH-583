#include <iostream>
#include <stack>
#include <random>

using namespace std;

int main() {
    // Method 1: Using std::rand() (older C-style)
    cout << "Method 1 (std::rand): Random number between 0 and 10: " 
              << rand() % 10 << endl;
    
    // Method 2: Using modern C++ random (recommended)
    random_device rd;  // Get a random seed from the OS
    mt19937 gen(rd()); // Standard mersenne_twister_engine
    uniform_int_distribution<> dis(0, 10); // Range [0, 10]
    
    cout << "Method 2 (modern C++): Random number between 0 and 10: " 
              << dis(gen) << endl;
    
    return 0;
}