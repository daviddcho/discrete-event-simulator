#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;
using json = nlohmann::json;

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

// Device status identifiers
bool CPU_BUSY;
bool DISK1_BUSY;
bool DISK2_BUSY;
bool NETWORK_BUSY;

// Global simulation clock
int current_time = INIT_TIME;

int ID = -1;

// This is the struct for event
typedef struct Event {
  int ID;               // job ID      
  int event_type;       // event type  
  int time_arrival;     // clock time (current_time + time)
}Event;

// This is the struct that implements the operator overloading
struct CompareTimeArrival {
  bool operator() (Event const& e1, Event const& e2) {
    return e1.time_arrival > e2.time_arrival;
  }
};

// Loads a config from json
void loadConfig(string filename) {
  ifstream file(filename);
  json config = json::parse(file);

  SEED = config["SEED"];
  INIT_TIME = config["INIT_TIME"];
  FIN_TIME  = config["FIN_TIME"];
  ARRIVE_MIN  = config["ARRIVE_MIN"];
  ARRIVE_MAX = config["ARRIVE_MAX"];
  QUIT_PROB = config["QUIT_PROB"];
  NETWORK_PROB = config["NETWORK_PROB"];
  CPU_MIN  = config["CPU_MIN"];
  CPU_MAX = config["CPU_MAX"];
  DISK1_MIN = config["DISK1_MIN"];
  DISK1_MAX = config["DISK1_MAX"];
  DISK2_MIN = config["DISK2_MIN"];
  DISK2_MAX = config["DISK2_MAX"];
  NETWORK_MIN = config["NETWORK_MIN"];
  NETWORK_MAX = config["NETWORK_MAX"];
}

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

// Calculates if job will leave system or not 
bool quitSystemProb() {
  //srand((unsigned) time(0)); 
  int random = rand() % 100 + 1;
  //cout << "system:" << random << '\n';
  if (random > QUIT_PROB) {
    // not leaving system
    return false;
  }
  // true, leaving system
  return true; 
}

// Calculates if job will go to network or disk
bool networkProb() {
  //srand((unsigned) time(0));
  int random = rand() % 100 + 1;
  //cout << "network:" << random << '\n';
  if (random > NETWORK_PROB) {
    // go to disk
    return false;
  }
  // go to network
  return true;
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

vector<Event> eventLog;
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

int maxSizeEQ = 0;
int maxSizeCPUQ = 0;
int maxSizeD1Q = 0;
int maxSizeD2Q = 0;
int maxSizeNQ = 0;

int totalSizeEQ = 0;
int totalSizeCPUQ = 0;
int totalSizeD1Q = 0;
int totalSizeD2Q = 0;
int totalSizeNQ = 0;

int countEQ = 0;
int countCPUQ = 0;
int countD1Q = 0;
int countD2Q = 0;
int countNQ = 0;

int totalUtilCPU = 0;
int totalUtilDISK1 = 0;
int totalUtilDISK2 = 0;
int totalUtilNETWORK = 0;

int maxRespTimeCPU = 0;
int maxRespTimeDISK1 = 0;
int maxRespTimeDISK2 = 0;
int maxRespTimeNETWORK = 0;

int totalRespTimeCPU = 0;
int totalRespTimeDISK1 = 0;
int totalRespTimeDISK2 = 0;
int totalRespTimeNETWORK = 0;

int countCRT = 0;
int countD1RT = 0;
int countD2RT = 0;
int countNRT = 0;

int thruPutCPU;
int thruPutDISK1;
int thruPutDISK2;
int thruPutNETWORK;


double getThruPutCPU() {
  return (double) countCPUQ / (double) FIN_TIME;
}
double getThruPutDISK1() {
  return (double) countD1Q / (double) FIN_TIME;
}
double getThruPutDISK2() {
  return (double) countD2Q / (double) FIN_TIME;
}
double getThruPutNETWORK() {
  return (double) countNQ / (double) FIN_TIME;
}


// Setting response time stats
void setRespTimeCPU(int respTime) {
  //cout << respTime << '\n';
  totalRespTimeCPU += respTime;
  countCRT++;
  if (respTime > maxRespTimeCPU) {
    maxRespTimeCPU = respTime;
  }
}
void setRespTimeDISK1(int respTime) {
  totalRespTimeDISK1 += respTime;
  countD1RT++;
  if (respTime > maxRespTimeDISK1) {
    maxRespTimeDISK1 = respTime;
  }
}
void setRespTimeDISK2(int respTime) {
  totalRespTimeDISK2 += respTime;
  countD2RT++;
  if (respTime > maxRespTimeDISK2) {
    maxRespTimeDISK2 = respTime;
  }
}
void setRespTimeNETWORK(int respTime) {
  totalRespTimeNETWORK += respTime;
  countNRT++;
  if (respTime > maxRespTimeNETWORK) {
    maxRespTimeNETWORK = respTime;
  }
}

// Storing total component util time
void setTotalUtilCPU(int time) {
  totalUtilCPU += time; 
}
void setTotalUtilDISK1(int time) {
  totalUtilDISK1 += time;
}
void setTotalUtilDISK2(int time) {
  totalUtilDISK2 += time;
}
void setTotalUtilNETWORK(int time) {
  totalUtilNETWORK += time;
}
// Calcuating component utilization
double getUtilCPU(int totalUtilCPU) {
  return ((double)totalUtilCPU / (double)FIN_TIME);
}
double getUtilDISK1(int totalUtilDISK1) {
  return ((double)totalUtilDISK1 / (double)FIN_TIME);
}
double getUtilDISK2(int totalUtilDISK2) {
return ((double)totalUtilDISK2 / (double)FIN_TIME);
}
double getUtilNETWORK(int totalUtilCPU) {
  return ((double)totalUtilNETWORK / (double)FIN_TIME);
}


// Setting values of Event Queue stats (total, count, max)
void setEQ(int size) {
  totalSizeEQ += size;
  countEQ++;
  if (size > maxSizeEQ) {
    maxSizeEQ = size;
  }
}
// Getting values of CPUQ stats (total, count, max)
void setCPUQ(int size) {
  totalSizeCPUQ += size;
  countCPUQ++;
  if (size > maxSizeCPUQ) {
    maxSizeCPUQ = size;
  }
}
// Getting values of D1Q stats (total, count, max)
void setD1Q(int size) {
  totalSizeD1Q += size;
  countD1Q++;
  if (size > maxSizeD1Q) {
    maxSizeD1Q = size;
  }
}
// Getting values of D2Q stats (total, count, max)
void setD2Q(int size) {
  totalSizeD2Q += size;
  countD2Q++;
  if (size > maxSizeD2Q) {
    maxSizeD2Q = size;
  }
}
// Getting values of NQ stats (total, count, max)
void setNQ(int size) {
  //cout << "size: " << size;
  totalSizeNQ += size;
  countNQ++;
  if (size > maxSizeNQ) {
    maxSizeNQ = size;
  }
}


void simStats() {
  ofstream stat("stats.txt");
  
  if (stat.is_open()) {
    stat << "---------Max and Avg Queue Size---------\n";
    stat << "Event Queue Max Size: " << maxSizeEQ << '\n'; 
    stat << "Event Queue Avg Size: " << (totalSizeEQ/countEQ) << '\n';
    stat << "CPU Queue Max Size: " << maxSizeCPUQ << '\n';
    stat << "CPU Queue Avg Size: " << (totalSizeCPUQ/countCPUQ) << '\n';
    stat << "DISK1 Queue Max Size: " << maxSizeD1Q << '\n';
    stat << "DISK1 Queue Avg Size: " << (totalSizeD1Q/countD1Q) << '\n';
    stat << "DISK2 Queue Max Size: " << maxSizeD1Q << '\n';
    stat << "DISK2 Queue Avg Size: " << (totalSizeD2Q/countD2Q) << '\n';
    stat << "NETWORK Queue Max Size: " << maxSizeNQ << '\n';
    stat << "NETWORK Queue Avg Size: " << (totalSizeNQ/countNQ) << '\n';
    stat << "---------Utilization---------\n";
    stat << "CPU Utilization: " << getUtilCPU(totalUtilCPU) << '\n';
    stat << "DISK1 Utilization: " << getUtilDISK1(totalUtilDISK1) << '\n';
    stat << "DISK2 Utilization: " << getUtilDISK2(totalUtilDISK2) << '\n';
    stat << "NETWORK Utilization: " << getUtilNETWORK(totalUtilNETWORK) << '\n';
    stat << "---------Response Time---------\n";
    stat << "CPU Max Response Time: " << maxRespTimeCPU << '\n';
    stat << "CPU Avg Response Time: " << (totalRespTimeCPU/countCRT) << '\n';
    stat << "DISK1 Max Response Time: " << maxRespTimeDISK1 << '\n';
    stat << "DISK1 Avg Response Time: " << (totalRespTimeDISK1/countD1RT) << '\n';
    stat << "DISK2 Max Response Time: " << maxRespTimeDISK2 << '\n';
    stat << "DISK2 Avg Response Time: " << (totalRespTimeDISK2/countD2RT) << '\n';
    stat << "NETWORK Max Response Time: " << maxRespTimeNETWORK << '\n';
    stat << "NETWORK Avg Response Time: " << (totalRespTimeNETWORK/countNRT) << '\n';
    stat << "---------Throughput---------\n";
    stat << "CPU Throughput: " << getThruPutCPU() << '\n'; 
    stat << "DISK1 Throughput: " << getThruPutDISK1() << '\n';
    stat << "DISK2 Throughput: " << getThruPutDISK2() << '\n';
    stat << "NETWORK Throughput: " << getThruPutNETWORK() << '\n';

  }
  stat.close();
}

// My component queues
queue<Event> CPU_queue;
queue<Event> DISK1_queue;
queue<Event> DISK2_queue;
queue<Event> NETWORK_queue;

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


int main () {
  // Loading config
  loadConfig("config.json"); 
  // Creating event queue 
  priority_queue<Event, vector<Event>, CompareTimeArrival> eventQueue;
  
  srand(SEED);
  
  eventQueue.push(newEvent(newID(), SIMULATION_END, FIN_TIME));
  eventQueue.push(newEvent(newID(), SYSTEM_ARRIVAL, INIT_TIME)); 
  Event event; 

  while (!eventQueue.empty() && current_time < FIN_TIME) {

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

void handleSYSTEM_ARRIVAL(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue) {
  // If theres something in the CPU queue or CPU is busy
  if (CPU_queue.size() != 0 || CPU_BUSY) { 
    CPU_queue.push(event); 
  } else { // if CPU is not busy add to CPU 
    
    // Calculating time of CPU Arrival
    int time_arrival = getTime(ARRIVE_MIN, ARRIVE_MAX);
    // Storing CPU response time
    setRespTimeCPU(time_arrival - current_time);
    
    eventQueue.push(newEvent(event.ID, CPU_ARRIVAL, time_arrival));
    //print_queue(eventQueue);
  }
  // Create a new SYSTEM_ARRIVAL event
  eventQueue.push(newEvent(newID(), SYSTEM_ARRIVAL, getTime(ARRIVE_MIN, ARRIVE_MAX)));
}

void handleSYSTEM_EXIT(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue) {
  // do nothing its leaving the system
}

void handleCPU_ARRIVAL(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue) {
  // CPU is now busy
  CPU_BUSY = true;
  
  // Calculating amount of time process will be in CPU
  int time_arrival = getTime(CPU_MIN, CPU_MAX);
  // Recording how long it will be in the CPU
  setTotalUtilCPU(time_arrival - current_time);

  // Create a new CPU_EXIT event (telling process to leave at finishing time)  
  eventQueue.push(newEvent(event.ID , CPU_EXIT, time_arrival));
}

void handleCPU_EXIT(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue) {
  // CPU is now not busy
  CPU_BUSY = false;
  
  // Recording CPU Queue size for stats
  setCPUQ(CPU_queue.size()); 

  // quit system probability
  if (quitSystemProb()) { 
    eventQueue.push(newEvent(event.ID, SYSTEM_EXIT, current_time));
  } else { // If stays in system 
    // If goes to network
    if(networkProb()) {
      if (NETWORK_queue.size() != 0 || NETWORK_BUSY) {
        NETWORK_queue.push(event);
      } else {
        int time_arrival = getTime(ARRIVE_MIN, ARRIVE_MAX);
        // Storing Network response time
        setRespTimeNETWORK(time_arrival - current_time);
        eventQueue.push(newEvent(event.ID, NETWORK_ARRIVAL, time_arrival));
      }
    } else { // If goes to DISK 
      
      // Go to the DISK with smaller size 
      if (DISK1_queue.size() <= DISK2_queue.size()) {
        // If DISK is busy, push to DISK queue
        if (DISK1_BUSY) {
          DISK1_queue.push(event); 
        } else { // If DISK is not busy, go to DISK
          int time_arrival = getTime(ARRIVE_MIN, ARRIVE_MAX);
          // Storing Disk1 response time
          setRespTimeDISK1(time_arrival - current_time);
          eventQueue.push(newEvent(event.ID, DISK1_ARRIVAL, time_arrival));
        }
      } else {
        // If DISK2 is busy, push to DISK2 queue
        if (DISK2_BUSY) {
          DISK2_queue.push(event);
        } else { // If DISK2 is not busy, go to DISK2
          int time_arrival = getTime(ARRIVE_MIN, ARRIVE_MAX);
          // Storing Disk2 response time
          setRespTimeDISK2(time_arrival - current_time);
          eventQueue.push(newEvent(event.ID, DISK2_ARRIVAL, time_arrival));
        }
      }
    }
  }
  
  // Check CPU FIFO queue
  // If there something in the CPU queue
  if (CPU_queue.size() != 0) {
    Event job = CPU_queue.front();
    CPU_queue.pop();
    
    // Storing CPU response time stats
    setRespTimeCPU(current_time - job.time_arrival);

    // Go to CPU with set usage time
    eventQueue.push(newEvent(job.ID, CPU_ARRIVAL, getTime(CPU_MIN, CPU_MAX)));
  }
}

void handleDISK1_ARRIVAL(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue) {
  // DISK1 is busy
  DISK1_BUSY = true;
  
  // Calculating how long process will use DISK1
  int time_arrival = getTime(DISK1_MIN, DISK1_MAX);
  // Storing usage time 
  setTotalUtilDISK1(time_arrival - current_time);

  // Create a new DISK1 event (telling process to leave at finishing time)
  eventQueue.push(newEvent(event.ID, DISK1_EXIT, time_arrival));
}

void handleDISK1_EXIT(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue) {
  // process exits disk, disk no longer busy
  DISK1_BUSY = false;
  
  // Recording DISK1 Queue size for stats
  setD1Q(DISK1_queue.size());

  // If there's a line for the CPU or if the CPU is BUSY
  // Push process into CPU queue
  if (CPU_queue.size() != 0 || CPU_BUSY) {
    CPU_queue.push(event);
  } else {
    // else send to CPU
    eventQueue.push(newEvent(event.ID, CPU_ARRIVAL, getTime(ARRIVE_MIN, ARRIVE_MAX)));
  }
  
  // Check DISK1 FIFO queue
  // If there something in the DISK1 queue
  if (DISK1_queue.size() != 0) {
    Event job = DISK1_queue.front();
    DISK1_queue.pop();

    // Storing CPU response time stats 
    setRespTimeDISK1(current_time - job.time_arrival);

    // Go to DISK1 with set usage time
    eventQueue.push(newEvent(job.ID, DISK1_ARRIVAL, getTime(DISK1_MIN, DISK1_MAX)));
  }
}

void handleDISK2_ARRIVAL(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue) {
  // DISK2 is busy
  DISK2_BUSY = true;
  
  // Calculating how long process wil use DISK2
  int time_arrival = getTime(DISK2_MIN, DISK2_MAX);
  // Sotring usage time
  setTotalUtilDISK2(time_arrival - current_time);

  // Create a new DISK2 EXIT event (telling process to leave at finishing time)
  eventQueue.push(newEvent(event.ID, DISK2_EXIT, time_arrival));
}

void handleDISK2_EXIT(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue) {
  // DISK2 is not busy
  DISK2_BUSY = false;
  
  // Recording DISK2 Queue size for stats
  setD2Q(DISK2_queue.size());
  
  // If there's a line for the CPU or if the CPU is busy
  // Push process into CPU queue
  if (CPU_queue.size() != 0 || CPU_BUSY) {
    CPU_queue.push(event);
  } else {
    // else send to CPU
    eventQueue.push(newEvent(event.ID, CPU_ARRIVAL, getTime(ARRIVE_MIN, ARRIVE_MAX)));
  }
  // Check DISK2 FIFO queue
  // If there something in the DISK2 queue
  if (DISK2_queue.size() != 0) {
    Event job = DISK2_queue.front();
    DISK2_queue.pop();

    // Storing CPU response time stats 
    setRespTimeDISK2(current_time - job.time_arrival);

    // Go to DISK1 with set usage time
    eventQueue.push(newEvent(job.ID, DISK2_ARRIVAL, getTime(DISK2_MIN, DISK2_MAX)));
  }

}

void handleNETWORK_ARRIVAL(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue) {
  // NETWORK is busy
  NETWORK_BUSY = true;
  
  // Calculating how long process will use NETWORK 
  int time_arrival = getTime(NETWORK_MIN, NETWORK_MAX);
  // Storing usage time
  setTotalUtilNETWORK(time_arrival - current_time);

  // Create a new NETWORK EXIT event (telling process to leave at finishing time)
  eventQueue.push(newEvent(event.ID, NETWORK_EXIT, time_arrival));
}

void handleNETWORK_EXIT(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue) {
  // NETWORK is not busy
  NETWORK_BUSY = false;
  
  // Recording NETWORK Queue size for stats
  setNQ(NETWORK_queue.size());

  // If there's a line for the CPU or if the CPU is busy
  // Push process into CPU queue
  if (CPU_queue.size() != 0 || CPU_BUSY) {
    CPU_queue.push(event);
  } else {
    // else send to CPU
    eventQueue.push(newEvent(event.ID, CPU_ARRIVAL, getTime(ARRIVE_MIN, ARRIVE_MAX)));
  }
  // Check DISK1 FIFO queue
  // If there something in the DISK1 queue
  if (NETWORK_queue.size() != 0) {
    Event job = NETWORK_queue.front();
    NETWORK_queue.pop();

    // Storing CPU response time stats 
    setRespTimeNETWORK(current_time - job.time_arrival);

    // Go to DISK1 with set usage time
    eventQueue.push(newEvent(job.ID, NETWORK_ARRIVAL, getTime(NETWORK_MIN, NETWORK_MAX)));
  }

}
void handleSIMULATION_END(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue) {
  // writes a log file
  logData(eventLog);
  simStats();
  // exit program
  exit(0);
}
