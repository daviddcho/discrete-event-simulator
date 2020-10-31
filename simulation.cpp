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

  while (!eventQueue.empty() && getCurrentTime() < getFIN_TIME()) {

    setEQ(eventQueue.size());

    event = eventQueue.top(); 
    eventQueue.pop();

    //current_time = event.time_arrival;
    addCurrentTime(event.time_arrival); 

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


