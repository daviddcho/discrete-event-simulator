#ifndef SIMULATION.H
#define SIMULATION_H

#include <iostream>

using namespace std;

// Event type values
const int SYSTEM_ARRIVAL = 0;
const int SYSTEM_EXIT = 1;
const int CPU_ARRIVAL = 2;
const int CPU_EXIT = 3;
const int DISK1_ARRIVAL = 4;
const int DISK1_EXIT = 5;
const int DISK2_ARRIVAL = 6;
const int DISK2_EXIT = 7;
const int NETWORK_ARRIVAL = 8;
const int NETWORK_EXIT = 9;
const int SIMULATION_END = 10;


// This is the struct for event
typedef struct Event {
  int ID;               // job ID      
  int event_type;       // event type  
  int time_arrival;     // clock time (current_time + time)
}Event;
// Creates a new event
Event newEvent(int ID, int event_type, int time_arrival); 


// This is the struct that implements the operator overloading
struct CompareTimeArrival {
  bool operator() (Event const& e1, Event const& e2) {
    return e1.time_arrival > e2.time_arrival;
  }
};


// Creates a new ID for new event
int newID();



#endif
