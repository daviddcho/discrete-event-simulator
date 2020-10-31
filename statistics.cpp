#include "simulation.h"



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

// Stat variables
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

