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
    TMP.b_rcv = b.b_rcv - a.b_rcv;
    TMP.b_tx = b.b_tx - a.b_tx;
    TMP.pk_rcv = b.pk_rcv - a.pk_rcv;
    TMP.pk_tx = b.pk_tx - a.pk_tx;
    return TMP;
}

inline STATS_ operator+(const STATS_ &b, const STATS_ &a)
{
    STATS_ TMP;
    TMP.b_rcv = b.b_rcv + a.b_rcv;
    TMP.b_tx = b.b_tx + a.b_tx;
    TMP.pk_rcv = b.pk_rcv + a.pk_rcv;
    TMP.pk_tx = b.pk_tx + a.pk_tx;
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

inline TFSTATS operator+(const TFSTATS &a, const TFSTATS &b)
{
    TFSTATS ret;
    ret.Statlist = a.Statlist + b.Statlist;
    if (a.DateTimeZulu > b.DateTimeZulu)
        ret.DateTimeZulu = a.DateTimeZulu;
    else
        ret.DateTimeZulu = b.DateTimeZulu;
    return ret;
}

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

/*
CheckStrValues
Checks a null-terminated string to ensure the contained characters are within a range from val_low to val_hi
Accepts: null terminated string, low character value, high character value
Returns: true if values are within limits, false if string is too long or if values are outside limits
*/
bool CheckStrValues(const char* str, char val_low, char val_hi);

/*
RefreshWait
Sleeps for the period defined by the REFRESH period
Accepts: nothing;
Returns: nothing after waiting for REFRESH time
*/
void RefreshWait();

/*
GetLine
Captures line from a file stream based on a delimiting character (or newline, or EOF, or null character)
Accepts: file stream f, null-terminated character delimiter
Returns: String captured between file stream iterator location and delim value
*/
std::string GetLine(std::fstream& f,char* delim);

/*
FindInString
Searches for a string within another string
Accepts: null terminated string to search, null terminated string to find, whether they need to exactly match, list of characters (delim)
Returns: integer location in the stringtosearch where the stringtofind is located
*/
int FindInString(const char* stringtosearch, const char* stringtofind, bool ExactMatch, const char* ignores);

#endif
