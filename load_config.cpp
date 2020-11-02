#include "simulation.h"
#include <nlohmann/json.hpp>
#include <fstream>
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


// Get functions for all the config variables
int getSEED() {
  return SEED;    
}

int getINIT_TIME() {
  return INIT_TIME;
}

int getFIN_TIME() {
  return FIN_TIME;
}

int getARRIVE_MIN() {
  return ARRIVE_MIN;
}

int getARRIVE_MAX() {
  return ARRIVE_MAX;
}

int getQUIT_PROB() {
  return QUIT_PROB;
}

int getNETWORK_PROB() {
  return NETWORK_PROB;
}

int getCPU_MIN() {
  return CPU_MIN;
}

int getCPU_MAX() {
  return CPU_MAX;
}

int getDISK1_MIN() {
  return DISK1_MIN;
}

int getDISK1_MAX() {
  return DISK1_MAX;
}

int getDISK2_MIN() {
  return DISK2_MIN;
}

int getDISK2_MAX() {
  return DISK2_MAX;
}

int getNETWORK_MIN() {
  return NETWORK_MIN;
}

int getNETWORK_MAX() {
  return NETWORK_MAX;
}

