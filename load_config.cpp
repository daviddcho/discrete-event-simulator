#include "simulation.h"
#include <nlohmann/json.hpp>
#include <fstream>
using json = nlohmann::json;

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

