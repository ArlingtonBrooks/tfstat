#ifndef __COMMON_HPP_
#define __COMMON_HPP_
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>

/*
Time structure
Stores year, month, day, hour, minute in smallest reasonable containers
Storage size 6 bytes
*/
struct TIME_ {
    short int year;
    char month;
    char day;
    char hour;
    char minute;
};

/*
TIME_ subtraction operator
Subtracts time structure t2 from t1
Returns: t2 - t1
*/
inline TIME_ operator-(const TIME_ &t2, const TIME_ &t1)
{
    TIME_ TMP;
    TMP.year = t2.year - t1.year;
    TMP.month = t2.month - t1.month;
    TMP.day = t2.day - t1.day;
    TMP.hour = t2.hour - t1.hour;
    TMP.minute = t2.minute - t1.minute;
    return TMP;
}

/*
Time comparison operators
Compares the states of TIME_ (based on 'largest is most recent/future')
Returns: boolean comparison
*/
bool operator>(TIME_ t1, const TIME_ t2);
bool operator<=(TIME_ t1, const TIME_ t2);
bool operator<(TIME_ t1, const TIME_ t2);
bool operator>=(TIME_ t1, const TIME_ t2);
bool operator==(TIME_ t1, const TIME_ t2);
bool operator!=(TIME_ t1, const TIME_ t2);

/*
GetTimeNow
Fills a TIME_ structure with the current time
Accepts: nothing
Returns: TIME_ structure representing current time
*/
TIME_ GetTimeNow();

/*
MakeFile
Make a file at location 'Filename'
Accepts: Filename location
Returns: true if able to create a file
*/
bool MakeFile(std::string Filename);

/*
STATS_ container
Stores values of bits/packets transmitted/received.
Storage size is 32 bytes
Theoretically able to track up to around 18 exabits
*/
struct STATS_{
    long unsigned int b_rcv;
    long unsigned int b_tx;
    long unsigned int pk_rcv;
    long unsigned int pk_tx;
};

/*
STATS_ subtraction operator
Subtracts STATS_ b from STATS_ a
Returns subtraction
*/
inline STATS_ operator-(const STATS_ &b, const STATS_ &a)
{
    STATS_ TMP;
    TMP.b_rcv = a.b_rcv - b.b_rcv;
    TMP.b_tx = a.b_tx - b.b_tx;
    TMP.pk_rcv = a.pk_rcv - b.pk_rcv;
    TMP.pk_tx = a.pk_tx - b.pk_tx;
    return TMP;
}

/*
STATS_ Boolean comparisons
Return boolean value depending on comparison result
*/
bool operator==(const STATS_ s1, const STATS_ s2);
bool operator!=(const STATS_ s1, const STATS_ s2);

/*
TFSTATS container
Contains a STATS_ value tied to a date/time value
*/
typedef struct {
    STATS_ Statlist;
    TIME_ DateTimeZulu;
} TFSTATS;

/*
IFACE_STAT Container
Contains an interface of name Iface, a boolean defining whether the interface is being actively monitored, and a TFSTATS object referring to the interface usage data
*/
typedef struct {
    std::string Iface;
    bool Active = true;
    TFSTATS Data;
} IFACE_STAT;

/*
Globally used variables
*/
extern bool DEBUG; //Whether to print debugging information
extern timespec REFRESH; //Length of time between data refresh
extern TIME_ Delta; //Amount of time between data saving to database
extern TIME_ History; //Length of time to keep any given stats entry
extern std::string DBase_Loc; //Location of databases
extern std::string DBase_Key_Loc; //Location of database keys
extern bool BitConvert; //Whether to convert output to bytes
extern bool SaveAll; //Whether to save all data (including duplicates)
extern std::vector<std::string> IFaceList; //List of all interfaces

/*
String List Parser
Parses a string line-by-line up to a delimiter to produce a vector of strings
Accepts: string list and null-terminated delimiter
Returns: vector of strings representing the string values before the delimiter
*/
std::vector<std::string> ParseStrList(std::string list, const char* delim);

#endif
