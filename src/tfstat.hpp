#ifndef __TFSTAT_HPP_
#define __TFSTAT_HPP_
#include <vector>
#include <string>
#include <algorithm>
//DateTime structure
//Inefficient compared to time_t, but guaranteed consistent;

#if defined(__linux__) || defined(__linux) || defined(linux)
//Simple structure to allow access to data from /proc/net/dev
typedef struct {
    std::string str;
    bool LOCK;
} PROCNET;

//Linux network count file /proc/net/dev
extern PROCNET PROC_NET_DEV;

/*
SetProcLock
Sets the 'Lock' state of PROC_NET_DEV variable
Accepts: the desired state to set PROC_NET_DEV.LOCK to
Returns: the final state of PROC_NET_DEV.LOCK
*/
bool SetProcLock(bool state);


//bool ReadNetStat(); //In src/Traffic
//std::vector<std::string> FindInterfaces(std::string data); //In src/Traffic
#endif

/*
 * Parse Options
 * --help: print quick help reference
 * --print-all: Print all stored data
 * -tl: print local time
 * -h xx: print past xx hours
 * -d xx: print past xx days
 * -w xx: print past xx weeks
 * -m xx: print past xx months
 * -y xx: print past xx years
 * -s INTERFACE: print current database size on disk
 * -t xx: number of minutes to wait before grabbing usage data.  Default 5; //SAVES IN 5 MINUTE INTERVALS TO DATABASE
 * -p INTERFACE: dump all information from INTERFACE in CSV format
 * To Be cont'd
*/
bool ParseArgs(int argc, char** argv);

/*
CheckStrValues
Checks a null-terminated string to ensure the contained characters are within a range from val_low to val_hi
Accepts: null terminated string, low character value, high character value
Returns: true if values are within limits, false if string is too long or if values are outside limits
*/
bool CheckStrValues(const char* str, char val_low, char val_hi);


//TIME_ GetTimeNow(); //src/Common

/*
RefreshWait
Sleeps for the period defined by the REFRESH period
Accepts: nothing;
Returns: nothing after waiting for REFRESH time
*/
void RefreshWait();

/*
ReadCFG
Reads configuration from the config file FNAME
Accepts: config file name, FNAME;
Returns: nothing after attempting to set global variables
*/
void ReadCFG(const char*);

/*
GetLine
Captures line from a file stream based on a delimiting character (or newline, or EOF, or null character)
Accepts: file stream f, null-terminated character delimiter
Returns: String captured between file stream iterator location and delim value
*/
std::string GetLine(std::fstream& f,char* delim);

//bool LoadDB(std::string Location, std::vector<TFSTATS> *DB);
//bool SaveDB(std::string Location, std::vector<TFSTATS> DB);

#endif
