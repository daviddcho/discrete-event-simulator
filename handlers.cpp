#include "simulation.h"

// Device status identifiers
bool CPU_BUSY;
bool DISK1_BUSY;
bool DISK2_BUSY;
bool NETWORK_BUSY;

// My component queues
queue<Event> CPU_queue;
queue<Event> DISK1_queue;
queue<Event> DISK2_queue;
queue<Event> NETWORK_queue;


void handleSYSTEM_ARRIVAL(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue) {
  // If theres something in the CPU queue or CPU is busy
  if (CPU_queue.size() != 0 || CPU_BUSY) { 
    CPU_queue.push(event); 
  } else { // if CPU is not busy add to CPU 
    
    // Calculating time of CPU Arrival
    int time_arrival = getTime(getARRIVE_MIN(), getARRIVE_MAX());
    // Storing CPU response time
    setRespTimeCPU(time_arrival - getCurrentTime());
    
    eventQueue.push(newEvent(event.ID, CPU_ARRIVAL, time_arrival));
    //print_queue(eventQueue);
  }
  // Create a new SYSTEM_ARRIVAL event
  eventQueue.push(newEvent(newID(), SYSTEM_ARRIVAL, getTime(getARRIVE_MIN(), getARRIVE_MAX())));
}

void handleSYSTEM_EXIT(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue) {
  // do nothing its leaving the system
}

void handleCPU_ARRIVAL(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue) {
  // CPU is now busy
  CPU_BUSY = true;
  
  // Calculating amount of time process will be in CPU
  int time_arrival = getTime(getCPU_MIN(), getCPU_MAX());
  // Recording how long it will be in the CPU
  setTotalUtilCPU(time_arrival - getCurrentTime());

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
    eventQueue.push(newEvent(event.ID, SYSTEM_EXIT, getCurrentTime()));
  } else { // If stays in system 
    // If goes to network
    if(networkProb()) {
      if (NETWORK_queue.size() != 0 || NETWORK_BUSY) {
        NETWORK_queue.push(event);
      } else {
        int time_arrival = getTime(getARRIVE_MIN(), getARRIVE_MAX());
        // Storing Network response time
        setRespTimeNETWORK(time_arrival - getCurrentTime());
        eventQueue.push(newEvent(event.ID, NETWORK_ARRIVAL, time_arrival));
      }
    } else { // If goes to DISK 
      
      // Go to the DISK with smaller size 
      if (DISK1_queue.size() <= DISK2_queue.size()) {
        // If DISK is busy, push to DISK queue
        if (DISK1_BUSY) {
          DISK1_queue.push(event); 
        } else { // If DISK is not busy, go to DISK
          int time_arrival = getTime(getARRIVE_MIN(), getARRIVE_MAX());
          // Storing Disk1 response time
          setRespTimeDISK1(time_arrival - getCurrentTime());
          eventQueue.push(newEvent(event.ID, DISK1_ARRIVAL, time_arrival));
        }
      } else {
        // If DISK2 is busy, push to DISK2 queue
        if (DISK2_BUSY) {
          DISK2_queue.push(event);
        } else { // If DISK2 is not busy, go to DISK2
          int time_arrival = getTime(getARRIVE_MIN(), getARRIVE_MAX());
          // Storing Disk2 response time
          setRespTimeDISK2(time_arrival - getCurrentTime());
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
    setRespTimeCPU(getCurrentTime() - job.time_arrival);

    // Go to CPU with set usage time
    eventQueue.push(newEvent(job.ID, CPU_ARRIVAL, getTime(getCPU_MIN(), getCPU_MAX())));
  }
}

void handleDISK1_ARRIVAL(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue) {
  // DISK1 is busy
  DISK1_BUSY = true;
  
  // Calculating how long process will use DISK1
  int time_arrival = getTime(getDISK1_MIN(), getDISK1_MAX());
  // Storing usage time 
  setTotalUtilDISK1(time_arrival - getCurrentTime());

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
    eventQueue.push(newEvent(event.ID, CPU_ARRIVAL, getTime(getARRIVE_MIN(), getARRIVE_MAX())));
  }
  
  // Check DISK1 FIFO queue
  // If there something in the DISK1 queue
  if (DISK1_queue.size() != 0) {
    Event job = DISK1_queue.front();
    DISK1_queue.pop();

    // Storing CPU response time stats 
    setRespTimeDISK1(getCurrentTime() - job.time_arrival);

    // Go to DISK1 with set usage time
    eventQueue.push(newEvent(job.ID, DISK1_ARRIVAL, getTime(getDISK1_MIN(), getDISK1_MAX())));
  }
}

void handleDISK2_ARRIVAL(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue) {
  // DISK2 is busy
  DISK2_BUSY = true;
  
  // Calculating how long process wil use DISK2
  int time_arrival = getTime(getDISK2_MIN(), getDISK2_MAX());
  // Sotring usage time
  setTotalUtilDISK2(time_arrival - getCurrentTime());

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
    eventQueue.push(newEvent(event.ID, CPU_ARRIVAL, getTime(getARRIVE_MIN(), getARRIVE_MAX())));
  }
  // Check DISK2 FIFO queue
  // If there something in the DISK2 queue
  if (DISK2_queue.size() != 0) {
    Event job = DISK2_queue.front();
    DISK2_queue.pop();

    // Storing CPU response time stats 
    setRespTimeDISK2(getCurrentTime() - job.time_arrival);

    // Go to DISK1 with set usage time
    eventQueue.push(newEvent(job.ID, DISK2_ARRIVAL, getTime(getDISK2_MIN(), getDISK2_MAX())));
  }

}

void handleNETWORK_ARRIVAL(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue) {
  // NETWORK is busy
  NETWORK_BUSY = true;
  
  // Calculating how long process will use NETWORK 
  int time_arrival = getTime(getNETWORK_MIN(), getNETWORK_MAX());
  // Storing usage time
  setTotalUtilNETWORK(time_arrival - getCurrentTime());

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
    eventQueue.push(newEvent(event.ID, CPU_ARRIVAL, getTime(getARRIVE_MIN(), getARRIVE_MAX())));
  }
  // Check DISK1 FIFO queue
  // If there something in the DISK1 queue
  if (NETWORK_queue.size() != 0) {
    Event job = NETWORK_queue.front();
    NETWORK_queue.pop();

    // Storing CPU response time stats 
    setRespTimeNETWORK(getCurrentTime() - job.time_arrival);

    // Go to DISK1 with set usage time
    eventQueue.push(newEvent(job.ID, NETWORK_ARRIVAL, getTime(getNETWORK_MIN(), getNETWORK_MAX())));
  }

}
void handleSIMULATION_END(Event &event, priority_queue<Event, vector<Event>, CompareTimeArrival> &eventQueue) {
  // writes a log file
  logData(getEventLog());
  simStats();
  // exit program
  exit(0);
}
