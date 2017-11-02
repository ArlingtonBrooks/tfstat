#ifndef __TFSTAT_HPP_
#define __TFSTAT_HPP_
#include <vector>
#include <string>
#include <algorithm>
//DateTime structure
//Inefficient compared to time_t, but guaranteed consistent;

#if defined(__linux__) || defined(__linux) || defined(linux)
typedef struct {
    std::string str;
    bool LOCK;
} PROCNET;

extern PROCNET PROC_NET_DEV;

bool SetProcLock(bool state);

bool ReadNetStat();
std::vector<std::string> FindInterfaces(std::string data);
#endif

bool ParseArgs(int argc, char** argv);
bool CheckStrValues(const char* str, char val_low, char val_hi);
TIME_ GetTimeNow();
void RefreshWait();

bool LoadDB(std::string Location, std::vector<TFSTATS> *DB);
bool SaveDB(std::string Location, std::vector<TFSTATS> DB);

#endif
