#include "simulation.h"
#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include <fstream>
#include <cstdlib>
#include <ctime>

#include <nlohmann/json.hpp>
#include <fstream>
using json = nlohmann::json;

using namespace std;

// Creates a new event
Event newEvent(int ID, int event_type, int time_arrival) {
  Event event;
  event.ID = ID;
  event.event_type = event_type;
  event.time_arrival = time_arrival;
  return event;
}

// creates a new ID for new event
int newID() {
  ID++;
  return ID;
}

// Gets time_arrival of event
int getTime(int min, int max) {
  //srand(SEED);
  int time = rand() % max + min;
  //cout << "time:" << time << '\n';
  time += current_time;
  
  return time;
}

// Prints event type 
string printEventType(Event event) {
  int type = event.event_type;
  if (type == 0) {
    return "arrived into the system";
  }
  else if (type == 1) {
    return "exited the system";
  }
  else if (type == 2) {
    return "arrived in CPU";
  }
  else if (type == 3) {
    return "exited the CPU";
  }
  else if (type == 4) {
    return "arrived in DISK1";
  }
  else if (type == 5) {
    return "exited DISK1";
  }
  else if (type == 6) {
    return "arrived in DISK2";
  }
  else if (type == 7) {
    return "exited DISK2";
  }
  else if (type == 8) {
    return "arrived in NETWORK";
  }
  else if (type == 9) {
    return "exited the NETWORK";
  }
  else {
    return "simulation ended";
  }
}


// Saves events of the simulation into a vector (ascending order) 
void captureEvent(Event event) {
  // inserts event into vector   
  eventLog.insert(eventLog.end(), event); 
}

// Writing to a log file with values of significant events
void logData(vector<Event> eventLog) {
  ofstream writelog("log.txt");
  ifstream file("config.json");
  json config = json::parse(file);

  if (writelog.is_open()) {
    writelog << "SEED: " << config["SEED"] << '\n';
    writelog << "INIT_TIME: " << config["INIT_TIME"] << '\n';
    writelog << "FIN_TIME: " << config["FIN_TIME"] << '\n';
    writelog << "ARRIVE_MIN: " << config["ARRIVE_MIN"] << '\n';
    writelog << "ARRIVE_MAX: " << config["ARRIVE_MAX"] << '\n';
    writelog << "QUIT_PROB: " << config["QUIT_PROB"] << '\n';
    writelog << "NETWORK_PROB: " << config["NETWORK_PROB"] << '\n';
    writelog << "CPU_MIN: " << config["CPU_MIN"] << '\n';
    writelog << "CPU_MAX: " << config["CPU_MAX"] << '\n';
    writelog << "DISK1_MIN: " << config["DISK1_MIN"] << '\n';
    writelog << "DISK1_MAX: " << config["DISK1_MAX"] << '\n';
    writelog << "DISK2_MIN: " << config["DISK2_MIN"] << '\n';
    writelog << "DISK2_MAX: " << config["DISK2_MAX"] << '\n';
    writelog << "NETWORK_MIN: " << config["NETWORK_MIN"] << '\n';
    writelog << "NETWORK_MAX: " << config["NETWORK_MAX"] << '\n';

    for (auto event : eventLog) {
      writelog << "At time: " << event.time_arrival << " Job" << event.ID << " " << printEventType(event) << '\n';
    }
  }
  writelog.close();
}
// Prints priority queue
void print_queue(priority_queue<Event, vector<Event>, CompareTimeArrival> eventQueue) {
  priority_queue<Event, vector<Event>, CompareTimeArrival> queue = eventQueue;
  Event event;
  while (!queue.empty()) {
    event = queue.top();
    cout << "EventID:" << event.ID << " event type:" << event.event_type << " time:" << event.time_arrival << '\n'; 
    queue.pop();
  }
}


int main () {
  // Loading config
  loadConfig("config.json"); 
  // Creating event queue 
  priority_queue<Event, vector<Event>, CompareTimeArrival> eventQueue;
  
  srand(getSEED());
  
  eventQueue.push(newEvent(newID(), SIMULATION_END, getFIN_TIME()));
  eventQueue.push(newEvent(newID(), SYSTEM_ARRIVAL, getINIT_TIME())); 
  Event event; 

  while (!eventQueue.empty() && current_time < getFIN_TIME()) {

    setEQ(eventQueue.size());

    event = eventQueue.top(); 
    eventQueue.pop();

    current_time = event.time_arrival;
    
    cout << "At time: " << event.time_arrival << " Job" << event.ID << " " << printEventType(event) << '\n';
    //cout << "Event ID:" << event.ID << " type:" << printEventType(event) << " time:" << event.time_arrival << '\n'; 
    // saves event into a vector
    captureEvent(event);

    switch(event.event_type) {
      case SYSTEM_ARRIVAL:
        handleSYSTEM_ARRIVAL(event, eventQueue);
        break;
      case SYSTEM_EXIT:
        handleSYSTEM_EXIT(event, eventQueue);
        break;
      case CPU_ARRIVAL:
        handleCPU_ARRIVAL(event, eventQueue);
        break;
      case CPU_EXIT:
        handleCPU_EXIT(event, eventQueue);
        break;
      case DISK1_ARRIVAL:
        handleDISK1_ARRIVAL(event, eventQueue);
        break;
      case DISK1_EXIT:
        handleDISK1_EXIT(event, eventQueue);
        break;
      case DISK2_ARRIVAL:
        handleDISK2_ARRIVAL(event, eventQueue);
        break;
      case DISK2_EXIT:
        handleDISK2_EXIT(event, eventQueue);
        break;
      case NETWORK_ARRIVAL:
        handleNETWORK_ARRIVAL(event, eventQueue);
        break;
      case NETWORK_EXIT:
        handleNETWORK_EXIT(event, eventQueue);
        break;
      case SIMULATION_END:
        handleSIMULATION_END(event, eventQueue);
        break;
    }

  }

  return 0;
}


