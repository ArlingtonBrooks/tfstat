#include "Common.hpp"

bool DEBUG = false;

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

bool MakeFile(std::string Filename)
{
    std::fstream f;
    f.open(Filename.c_str(),std::ios::out|std::ios::app);
    if (!f)
        return false;
    f.close();
    return true;
}

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
