#include "simulation.h"
#include <nlohmann/json.hpp>
#include <fstream>
using json = nlohmann::json;


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

