#include "simulation.h"




// Global event ID
int ID = -1;

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

int getID() {
  return ID;
}
