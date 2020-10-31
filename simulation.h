#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include <fstream>
#include <cstdlib>
#include <ctime>


using namespace std;

// Global variables for now
int SEED;
int INIT_TIME;
int FIN_TIME;
int ARRIVE_MIN;     
int ARRIVE_MAX;    
int QUIT_PROB;      // probability that it will quit the system (or another loop)
int NETWORK_PROB;   // probability it will go to network (or disk)
int CPU_MIN;      
int CPU_MAX;     
int DISK1_MIN;  
int DISK1_MAX; 
int DISK2_MIN;
int DISK2_MAX;     
int NETWORK_MIN; 
int NETWORK_MAX;


// Global simulation clock
int current_time = INIT_TIME;
// Global event ID
int ID = -1;


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

// Load config
void loadConfig(string filename); 

// Log simulation
vector<Event> eventLog;
void captureEvent(Event event);
void logData(vector<Event> eventLog);
string printEventType(Event event); 


#endif
