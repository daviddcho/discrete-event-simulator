#include "simulation.h"

int current_time = getINIT_TIME();


int getCurrentTime() {
  return current_time;
}


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

