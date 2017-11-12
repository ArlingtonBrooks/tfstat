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
