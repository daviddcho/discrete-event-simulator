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


// Get functions for config variables
int getSEED();
int getINIT_TIME();
int getFIN_TIME();
int getARRIVE_MIN();
int getARRIVE_MAX();
int getQUIT_PROB(); 
int getNETWORK_PROB();
int getCPU_MIN(); 
int getCPU_MAX();
int getDISK1_MIN(); 
int getDISK1_MAX(); 
int getDISK2_MIN(); 
int getDISK2_MAX(); 
int getNETWORK_MIN(); 
int getNETWORK_MAX(); 


// Global simulation clock
int getCurrentTime();
int setCurrentTime(int time);


// Returns a random time between the min and max
int getTime(int min, int max);


// Global event ID
int getID();
int newID();


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
vector<Event> getEventLog();
void captureEvent(Event event);
void logData(vector<Event> eventLog);
string printEventType(Event event); 


// Component Handlers
void handleSYSTEM_ARRIVAL(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue); 
void handleSYSTEM_EXIT(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue);
void handleCPU_ARRIVAL(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue); 
void handleCPU_EXIT(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue);
void handleDISK1_ARRIVAL(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue); 
void handleDISK1_EXIT(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue);
void handleDISK2_ARRIVAL(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue); 
void handleDISK2_EXIT(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue); 
void handleNETWORK_ARRIVAL(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue);
void handleNETWORK_EXIT(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue); 
void handleSIMULATION_END(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue);

// Stat 
bool quitSystemProb();
bool networkProb();
double getThruPutCPU();
double getThruPutDISK1();
double getThruPutDISK2();
double getThruPutNETWORK();
void setRespTimeCPU(int respTime);
void setRespTimeDISK1(int respTime);
void setRespTimeDISK2(int respTime);
void setRespTimeNETWORK(int respTime);
void setTotalUtilCPU(int time);
void setTotalUtilDISK1(int time);
void setTotalUtilDISK2(int time);
void setTotalUtilNETWORK(int time);
void setEQ(int size);
void setCPUQ(int size);
void setD1Q(int size);
void setD2Q(int size);
void setNQ(int size);
void simStats();


// Print priority queue (used for testing)
void print_queue(priority_queue<Event, vector<Event>, CompareTimeArrival> eventQueue); 


#endif
