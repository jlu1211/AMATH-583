#include "hw6-elevator.hpp"
#include <iostream>
#include <vector>
#include <thread>

int main() {
    // Number of people to simulate
    npeople = 20;  // You can adjust this number
    
    // Create vector to store elevator threads
    vector<thread> elevator_threads;
    
    // Create and start 6 elevator threads
    for (int i = 0; i < NUM_ELEVATORS; i++) {
        elevator_threads.emplace_back(elevator, i);
    }
    
    // Create and start person threads
    vector<thread> person_threads;
    for (int i = 0; i < npeople; i++) {
        person_threads.emplace_back(person, i);
    }
    
    // Wait for all person threads to complete
    for (auto& t : person_threads) {
        t.join();
    }
    
    // Wait for all elevator threads to complete
    for (auto& t : elevator_threads) {
        t.join();
    }
    
    // Print final statistics
    cout << "\nFinal Statistics:" << endl;
    for (int i = 0; i < NUM_ELEVATORS; i++) {
        cout << "Elevator " << i << " transported " << global_passengers_serviced[i] << " passengers" << endl;
    }
    
    return 0;
} 