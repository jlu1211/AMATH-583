// multiple rider case attempt
/*
Passenger List: elevator maintains list of passengers (pairs of person_id and destination floor)
Batch Processing: elevator picks up as many passengers as possible from the queue if they are on the same floor, respecting the maximum occupancy
Route Management: elevator manages passengers' routes, moving to each passenger's destination floor and logging the necessary information
Complete Logging: detailed logs for each step including entering and exiting the elevator are added to provide comprehensive traceability
also
Occupancy Declaration: occupancy variable initialized to 0 at the start of the elevator function
Occupancy Management: increment occupancy each time a passenger enters the elevator and decrement each time a passenger exits.
*/

#ifndef ELEVATOR_HPP
#define ELEVATOR_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include <random>
#include <atomic>
#include <vector>
#include <condition_variable>
#include <tuple>
#include <unordered_map>

using namespace std;

const int NUM_FLOORS = 50;
const int NUM_ELEVATORS = 6;
const int MAX_OCCUPANCY = 10;
const int MAX_WAIT_TIME = 5000; // milliseconds
const int TRAVEL_DELAY_MS = 0; // milliseconds
// Shared data structures
mutex cout_mtx; // for thread-safe cout
mutex queue_mtx; // for thread-safe queue operations
condition_variable cv; // for thread-safe condition variable
queue<tuple<int, int, int>> global_queue; // person_id, start_floor, dest_floor

vector<int> elevator_positions(NUM_ELEVATORS, 0);
atomic<int> num_people_serviced(0);
vector<int> global_passengers_serviced(NUM_ELEVATORS, 0);
int npeople;

// Simulate the time to move between floors
inline void drive(int id, int &current_floor, int target_floor) {
    if (current_floor == target_floor) return;
    int step = (target_floor > current_floor ? +1 : -1);
    while (current_floor != target_floor) {
        int next_floor = current_floor + step;
        {
            lock_guard<mutex> lock(cout_mtx);
            // cout << "Elevator " << id << " is moving from floor " << current_floor << " to floor " << next_floor << endl;
        }
        // Simulate the time to move between floors
        this_thread::sleep_for(chrono::milliseconds(TRAVEL_DELAY_MS));
        current_floor = next_floor;
    }
}


void elevator(int id) {
    int occupancy = 0; // Initialize occupancy for each elevator
    int current_floor = 0;
    vector<pair<int, int>> passengers; // pairs of (person_id, dest_floor)

    // please complete the code segment
    while (true) {
        unique_lock<mutex> lock(queue_mtx);
        cv.wait(lock, []{return !global_queue.empty() || num_people_serviced.load() >= npeople;});
        // If all done and no more requests, exit
        if (num_people_serviced.load() >= npeople && global_queue.empty()) {
            break;
        }

        // Pop one request from the queue        
        auto [person_id, start_floor, dest_floor] = global_queue.front();
        global_queue.pop();
        lock.unlock();
        
        // Drive to pick up passengers
        drive(id, current_floor, start_floor);

        // Person enters
        {
            lock_guard<mutex> lock(cout_mtx);
            cout << "Person " << person_id << " enters elevator " << id << " on floor " << start_floor << endl;
        }
        occupancy++;
        passengers.emplace_back(person_id, dest_floor);

        // Drive to destination
        drive(id, current_floor, dest_floor);
        {
            lock_guard<mutex> lock2(cout_mtx);
            cout << "Person " << person_id << " exits elevator " << id << " on floor " << dest_floor << endl;
        }
        occupancy--;
        passengers.erase(passengers.begin());

        // Update global stats
        global_passengers_serviced[id]++;
        num_people_serviced.fetch_add(1);

        cv.notify_all();
    }
    {
        lock_guard<mutex> lock(cout_mtx);
        cout << "Elevator " << id << " has finished servicing all people." << endl;
        cout << "Elevator " << id << " transported " << global_passengers_serviced[id] << " passengers." << endl;
    }
}

void person(int id) {
    int curr_floor = rand() % NUM_FLOORS;
    int dest_floor = rand() % NUM_FLOORS;
    while (dest_floor == curr_floor) {
        dest_floor = rand() % NUM_FLOORS;
    }

    {
        lock_guard<mutex> lock(cout_mtx);
        cout << "Person " << id << " wants to go from floor " << curr_floor << " to floor " << dest_floor << endl;
    }
    // Add person to queue
    {
        lock_guard<mutex> lock(queue_mtx);
        global_queue.emplace(id, curr_floor, dest_floor);
    }
    cv.notify_one();
}

#endif // ELEVATOR_HPP

