#include "simulation.h"


// Global current time
int current_time = getINIT_TIME();


// Returns the current time
int getCurrentTime() {
  return current_time;
}


// Sets current time to given time and returns current time
int setCurrentTime(int time) {
  current_time = time;  
  return current_time;
}


// Gets time_arrival of event
int getTime(int min, int max) {
  //srand(SEED);
  int time = rand() % max + min;
  time += getCurrentTime();
  
  return time;
}

