#ifndef __COMMON_HPP_
#define __COMMON_HPP_
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>

struct TIME_ {
    short int year;
    char month;
    char day;
    char hour;
    char minute;
};

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

bool operator>(TIME_ t1, const TIME_ t2);
bool operator<=(TIME_ t1, const TIME_ t2);
bool operator<(TIME_ t1, const TIME_ t2);
bool operator>=(TIME_ t1, const TIME_ t2);
bool operator==(TIME_ t1, const TIME_ t2);
bool operator!=(TIME_ t1, const TIME_ t2);

TIME_ GetTimeNow();
bool MakeFile(std::string Filename);

//Stats struct
struct STATS_{
    long unsigned int b_rcv;
    long unsigned int b_tx;
    long unsigned int pk_rcv;
    long unsigned int pk_tx;
};

inline STATS_ operator-(const STATS_ &b, const STATS_ &a)
{
    STATS_ TMP;
    TMP.b_rcv = a.b_rcv - b.b_rcv;
    TMP.b_tx = a.b_tx - b.b_tx;
    TMP.pk_rcv = a.pk_rcv - b.pk_rcv;
    TMP.pk_tx = a.pk_tx - b.pk_tx;
    return TMP;
}

bool operator==(const STATS_ s1, const STATS_ s2);
bool operator!=(const STATS_ s1, const STATS_ s2);

typedef struct {
    STATS_ Statlist;
    TIME_ DateTimeZulu;
} TFSTATS;

typedef struct {
    std::string Iface;
    bool Active = true;
    TFSTATS Data;
} IFACE_STAT;

extern bool DEBUG;
extern timespec REFRESH;
extern TIME_ Delta;
extern TIME_ History;
extern std::string DBase_Loc;
extern std::string DBase_Key_Loc;
extern bool BitConvert;
extern bool SaveAll;
extern std::vector<std::string> IFaceList;

std::vector<std::string> ParseStrList(std::string list, const char* delim);

#endif
