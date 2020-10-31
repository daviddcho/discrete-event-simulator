#include "simulation.h"

int current_time = getINIT_TIME();

int getCurrentTime() {
  return current_time;
}

int addCurrentTime(int time) {
  return current_time + time;
}
