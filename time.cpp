#include "simulation.h"

int current_time = getINIT_TIME();


int getCurrentTime() {
  return current_time;
}


int addCurrentTime(int time) {
  return current_time + time;
}


// Gets time_arrival of event
int getTime(int min, int max) {
  //srand(SEED);
  int time = rand() % max + min;
  //cout << "time:" << time << '\n';
  time += getCurrentTime();
  
  return time;
}

