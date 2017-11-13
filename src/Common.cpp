#include "Common.hpp"

/*
Global debug variable
*/
bool DEBUG = false;

/*
TIME_ t1 > TIME_ t2
*/
bool operator>(TIME_ t1, const TIME_ t2)
{
    if (t1.year > t2.year)
        return true;
    else if (t1.year < t2.year)
        return false;
    else
    {
        if (t1.month > t2.month)
            return true;
        else if (t1.month < t2.month)
            return false;
        else
        {
            if (t1.day > t2.day)
                return true;
            else if (t1.day < t2.day)
                return false;
            else
            {
                if (t1.hour > t2.hour)
                    return true;
                else if (t1.hour < t2.hour)
                    return false;
                else
                {
                    if (t1.minute > t2.minute)
                        return true;
                    else if (t1.minute < t2.minute)
                        return false;
                    else
                        return false;
                }
            }
        }
    }
}

/*
TIME_ t1 < TIME_ t2
*/
bool operator<(TIME_ t1, const TIME_ t2)
{
    if (t1.year > t2.year)
        return false;
    else if (t1.year < t2.year)
        return true;
    else
    {
        if (t1.month > t2.month)
            return false;
        else if (t1.month < t2.month)
            return true;
        else
        {
            if (t1.day > t2.day)
                return false;
            else if (t1.day < t2.day)
                return true;
            else
            {
                if (t1.hour > t2.hour)
                    return false;
                else if (t1.hour < t2.hour)
                    return true;
                else
                {
                    if (t1.minute > t2.minute)
                        return false;
                    else if (t1.minute < t2.minute)
                        return true;
                    else
                        return false;
                }
            }
        }
    }
}

/*
TIME_ t1 >= TIME_ t2
*/
bool operator>=(TIME_ t1, const TIME_ t2)
{
    if (t1.year > t2.year)
        return true;
    else if (t1.year < t2.year)
        return false;
    else
    {
        if (t1.month > t2.month)
            return true;
        else if (t1.month < t2.month)
            return false;
        else
        {
            if (t1.day > t2.day)
                return true;
            else if (t1.day < t2.day)
                return false;
            else
            {
                if (t1.hour > t2.hour)
                    return true;
                else if (t1.hour < t2.hour)
                    return false;
                else
                {
                    if (t1.minute > t2.minute)
                        return true;
                    else if (t1.minute < t2.minute)
                        return false;
                    else
                        return true;
                }
            }
        }
    }
}

/*
TIME_ t1 <= TIME_ t2
*/
bool operator<=(TIME_ t1, const TIME_ t2)
{
    if (t1.year > t2.year)
        return false;
    else if (t1.year < t2.year)
        return true;
    else
    {
        if (t1.month > t2.month)
            return false;
        else if (t1.month < t2.month)
            return true;
        else
        {
            if (t1.day > t2.day)
                return false;
            else if (t1.day < t2.day)
                return true;
            else
            {
                if (t1.hour > t2.hour)
                    return false;
                else if (t1.hour < t2.hour)
                    return true;
                else
                {
                    if (t1.minute > t2.minute)
                        return false;
                    else if (t1.minute < t2.minute)
                        return true;
                    else
                        return true;
                }
            }
        }
    }
}

/*
TIME_ t1 == TIME_ t2
*/
bool operator==(TIME_ t1, const TIME_ t2)
{
    return (t1.year == t2.year && t1.month == t2.month && t1.day == t2.day && t1.hour == t2.hour && t1.minute == t2.minute);
}

/*
TIME_ t1 != TIME_ t2
*/
bool operator!=(TIME_ t1, const TIME_ t2)
{
    return !(t1 == t2);
}

/*
STATS_ s1 == STATS_ s2
*/
bool operator==(const STATS_ s1, const STATS_ s2)
{
    return (s1.b_rcv == s2.b_rcv && s1.b_tx == s2.b_tx && s1.pk_rcv == s2.pk_rcv && s1.pk_tx == s2.pk_tx);
}

/*
STATS_ s1 != STATS_ s2
*/
bool operator!=(const STATS_ s1, const STATS_ s2)
{
    return !(s1 == s2);
}

/*
Make File at Filename
*/
bool MakeFile(std::string Filename)
{
    std::fstream f;
    f.open(Filename.c_str(),std::ios::out|std::ios::app);
    if (!f)
        return false;
    f.close();
    return true;
}

/*
Get current time UTC
*/
TIME_ GetTimeNow()
{
    time_t Cur = time(0);
    struct tm *ptm;
    ptm = gmtime(&Cur);
    if (DEBUG)
        fprintf(stderr,"Got time: \nYR: %d, MO: %d, DA: %d, HR: %d\n",ptm->tm_year,ptm->tm_mon,ptm->tm_mday,ptm->tm_hour);
    TIME_ ret;
    ret.year = ptm->tm_year;
    ret.month = ptm->tm_mon;
    ret.day = ptm->tm_mday;
    ret.hour = ptm->tm_hour;
    ret.minute = ptm->tm_min;
    return ret;
}

/*
Parse a string into a vector based on a character delimiter
*/
std::vector<std::string> ParseStrList(std::string list, const char* delim)
{
    std::vector<std::string> ret;
    std::string tmp;
    for (int i = 0; i < list.length(); i++)
    {
        bool skip = false;
        int j = 0;
        while (delim[j] != '\0')
        {
            if (list[i] == delim[j])
                skip = true;
            j++;
        }
        if (!skip)
            tmp += list[i];
        else
        {
            ret.push_back(tmp);
            tmp.clear();
        }
    }
    ret.push_back(tmp);
    return ret;
}

/*
CheckStrValues
*/
bool CheckStrValues(const char* str, char val_low, char val_hi)
{
    int j = 0;
    while (str[j] != NULL)
    {
        if (str[j] < val_low || str[j] > val_hi)
            return false;
        j += 1;
        if (j < 0)
            return false; //too long to check;
    }
    return true;
}

/*
RefreshWait
*/
void RefreshWait()
{
    timespec TS = REFRESH;
    clock_nanosleep(CLOCK_MONOTONIC,0,&TS,NULL);
}

/*
GetLine
*/
std::string GetLine(std::fstream& f,char* delim = NULL)
{
    std::string ret;
    char Buffer = f.get();
    bool LOOP = true;

    if (delim != NULL) //check for delimiter
    {
        int i = 0;
        while (delim[i] != '\0')
        {
            if (Buffer == delim[i])
                LOOP = false;
            i++;
        }
    }

    while (f.good() && Buffer != EOF && LOOP) //TODO: more reliable
    {
        ret += Buffer;
        Buffer = f.get();
        if (delim != NULL) //check for delimiter
        {
            int i = 0;
            while (delim[i] != '\0')
            {
                if (Buffer == delim[i])
                    LOOP = false;
                i++;
            }
        }
    }
    return ret;
}

/*
FindInString
*/
int FindInString(const char* searchtext, const char* searchfor, bool ExactMatch, const char* ignores)
{
    int i = 0;
    if (searchtext == NULL || searchfor == NULL)
        return -1;
    while (searchtext[i] != NULL)
    {
        if (searchtext[i] == searchfor[0])
        {
            //test the rest of the string
            int j = 0;
            while (searchfor[j] != NULL)
            {
                if (searchtext[i+j] == searchfor[j])
                {
                    j++;
                    continue;
                }
                else
                {
                    i++;
                    break;
                }
            }
            if (searchfor[j] == NULL)
            {
                if (!ExactMatch)
                    return i+j;
                else
                {
                    int k = 0;
                    while (ignores[k] != NULL)
                    {
                        if (searchtext[i+j] == ignores[k])
                            return i+j;
                        k++;
                    }
                }
                i++;
            }
        }
        else
            i++;
    }
    return -1;
}

