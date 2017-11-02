/*
BSD 2-Clause License

Copyright (c) 2017, Kevin Arlington Brooks
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//#include <net/if.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <csignal>
#include "Common.hpp"
//#include "Common.cpp"
#include "tfstat.hpp"
#include "traffic.hpp"
#include "defaults.hpp"
#include "Signals.cpp"
#include "Database.hpp"
//#include "traffic.cpp" //rem;

#if defined(__linux__) || defined(__linux) || defined(linux)
PROCNET PROC_NET_DEV = {"",false};
bool SetProcLock(bool state)
{
    int iter = 0;
    if (state == true)
        while (PROC_NET_DEV.LOCK && iter < 100000000)
            iter++;
    if (iter >= 100000000-1)
        fprintf(stderr,"Failed to lock /proc/net/dev\n");
    PROC_NET_DEV.LOCK = state;
}
#endif

/*
 * Theoretical storage requirements:
 * 5 years taking 1 value per minute: Each point generates about a 64-byte structure
 * results in 168192000 bytes used, or about 168 Megabytes.
*/

int main (int argc, char** argv)
{
    DEBUG = true;
    ParseArgs(argc,argv);
    bool Running = true;
    std::vector<IFACE_STAT> Statlist;

    Statlist = InitIface();
//    ReadNetStat();
//    GetState("enp2s0");

    if (OneShot)
        Running = false;

    SetExitHandler(SIGTERM);
    SetExitHandler(SIGINT);
    SetExitHandler(SIGQUIT);
/*FIXME: Temporarily disabled program;
    DumpDatabase("tfstat.dbenp2s0.db");
    DumpKeys("tfstat.dbenp2s0.tbl");
    
    std::vector<std::vector<DBASE_ENTRY>> Databases;
    for (int i = 0; i < Statlist.size(); i++)
    {
        Databases.push_back(LoadDB(Base_Dir + Base_DBase_Loc + Statlist[i].Iface + DBase_Key_Ext)); //NEed a single file for each IFACE;
    }
    while (Running)
    {
        RefreshWait();
        ReadNetStat();

        for (int i = 0; i < Statlist.size(); i++)
        {
            if (Statlist[i].Active)
            {//FIXME: when do we update statlist?
                TFSTATS st = {GetState(Statlist[i].Iface),GetTimeNow()};
                if (Databases[i].size() > 0)
                {
                    if (st.DateTimeZulu - Statlist[i].Data.DateTimeZulu >= Delta)
                    {
                        Statlist[i].Data.DateTimeZulu = st.DateTimeZulu;
                        Databases[i] = SaveToDB(Base_Dir + Base_DBase_Loc + Statlist[i].Iface + DBase_Ext,Base_Dir + Base_DBase_Loc + Statlist[i].Iface + DBase_Key_Ext,st,Databases[i]);
                    }
                }
                else
{
                    Statlist[i].Data.DateTimeZulu = st.DateTimeZulu;
printf("DBS: %d\n",Databases[i].size());
                    Databases[i] = SaveToDB(Base_Dir + Base_DBase_Loc + Statlist[i].Iface + DBase_Ext,Base_Dir + Base_DBase_Loc + Statlist[i].Iface + DBase_Key_Ext,st,Databases[i]);
}
            }
        }
        //LOAD FROM FILE?
        if (IsExiting)
            Running = false;
    }
    //SaveDBK("DKEY",Databases);
    for (int i = 0; i < Statlist.size(); i++)
    {
        if (Statlist[i].Active)
        {
            TFSTATS st = {GetState(Statlist[i].Iface),GetTimeNow()};
            if (Databases.size() > 0)
            {
                Databases[i] = SaveToDB(Base_Dir + Base_DBase_Loc + Statlist[i].Iface + DBase_Ext,Base_Dir + Base_DBase_Loc + Statlist[i].Iface + DBase_Key_Ext,st,Databases[i]);
            }
        }
    }*/
    printf("SAVED\n");
    return EXIT_SUCCESS;
    //NEED LIST OF INTERFACES TO SCAN FOR!
}

/*
 * Parse Options
 * --help: print quick help reference
 * --print-all: Print all stored data
 * -tl: use local time
 * -h xx: print past xx hours
 * -d xx: print past xx days
 * -w xx: print past xx weeks
 * -m xx: print past xx months
 * -y xx: print past xx years
 * -s INTERFACE: print current database size on disk
 * -t xx: number of minutes to wait before grabbing usage data.  Default 5; //SAVES IN 5 MINUTE INTERVALS TO DATABASE
 * -p INTERFACE: dump all information from INTERFACE in CSV format
 * -i INTERFACE: activate INTERFACE capture
 * -x INTERFACE: Deactivate INTERFACE capture
 * To Be cont'd
*/
bool ParseArgs(int argc, char** argv)
{
    printf("Parse command line options: NOT YET IMPLEMENTED\n");
    if (DEBUG)
        fprintf(stderr,"Parsing command line arguments: %s\n",argv[0]);

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i],"--help") == 0)
            printf("Help Printout");
        else if (strcmp(argv[i],"--print-all") == 0)
            printf("Print All Data");
        else if (strcmp(argv[i],"-h") == 0) //pring hourly
        {
            printf("Return past %s hours\n",argv[i+1]);
            i += 1;
            continue;
        }
        else if (strcmp(argv[i],"-d") == 0) //print daily
        {
            printf("Return past %s days\n",argv[i+1]);
            i += 1;
            continue;
        }
        else if (strcmp(argv[i],"-w") == 0) //print weekly
        {
            printf("Return past %s weeks\n",argv[i+1]);
            i += 1;
            continue;
        }
        else if (strcmp(argv[i],"-m") == 0) //print monthly
        {
            printf("Return past %s months\n",argv[i+1]);
            i += 1;
            continue;
        }
        else if (strcmp(argv[i],"-y") == 0) //print yearly
        {
            printf("Return past %s years\n",argv[i+1]);
            i += 1;
            continue;
        }
        else if (strcmp(argv[i],"-s") == 0) //print amount of data
        {
            //Check for database in default directory
            i += 1;
            struct stat stat_buf1, stat_buf2;
            int rc = stat((Base_Dir + Base_DBase_Loc + std::string(argv[i]) + DBase_Ext).c_str(),&stat_buf1);
            if (rc < 0)
            {
                perror("Failed to calculate database size");
                continue;
            }
            int rc2 = stat((Base_Dir + Base_DBase_Loc + std::string(argv[i]) + DBase_Key_Ext).c_str(),&stat_buf2);
            if (rc2 < 0)
            {
                perror("Failed to calculate database size");
                continue;
            }
            printf("Interface %s:\nDatabase Size: %ld Bytes\nLookup Table Size: %ld Bytes\n",argv[i],stat_buf1.st_size,stat_buf2.st_size);

            printf("Size on disk: %ld Bytes\n",stat_buf1.st_size + stat_buf2.st_size);
            continue;
        }
        else if (strcmp(argv[i],"-t") == 0) //FIXME: check for invalid input also;
        {
            printf("Parsing -t %s\n",argv[i+1]);
            i += 1;
            int min;
            int Succeeds = sscanf(argv[i]," %d ",&min);
            if (!CheckStrValues(argv[i],'0','9'))
                Succeeds = -1;
            if (min > 10000 || min <= 0 || Succeeds != 1)
            {
                fprintf(stderr,"ERROR: Specified timeframe for -t %d is invalid.  Times must be between 0 and 10000 minutes\n",min);
                OneShot = true;
                continue;
            }
            else
            {
                int Days = min/1440;
                int Hours = (min-Days*1440)/60;
                int Minutes = (min-Hours*60-Days*1440);
                TIME_ TVal = {0,0,Days,Hours,Minutes};
                if (DEBUG)
                    fprintf(stderr,"Time from command line: %d days, %d hours, %d minutes\n",Days,Hours,Minutes);
                continue;
            }
        }
        else if (strcmp(argv[i],"-p") == 0)
        {
            i += 1;
            DumpDatabase(Base_Dir + Base_DBase_Loc + std::string(argv[i]) + DBase_Ext);
            continue;
        }
    }
}

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

/*TIME_ GetTimeNow()
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
}*/

/*#if defined(__linux__) || defined(__linux) || defined(linux)
//Update PROC_NET_DEV;
bool ReadNetStat()
{
    if (DEBUG)
        fprintf(stderr,"Reading network status\n");
    std::fstream f;
    f.open("/proc/net/dev",std::fstream::in);

    SetProcLock(true);
    PROC_NET_DEV.str.clear();

    while (f.good()) //FIXME: another way to avoid errors?
    {
        PROC_NET_DEV.str += f.get();
    }
    SetProcLock(false);

    if (DEBUG)
        fprintf(stderr,"Read network status: \n",PROC_NET_DEV.str.c_str());

    f.close();

    return true;
}
#endif*/

//Automatically build list of interfaces/
/*std::vector<std::string> FindInterfaces(std::string data)
{
    if (DEBUG)
        fprintf(stderr,"Scanning for interfaces...\n");
    std::istringstream f(data);
    std::vector<std::string> interfaces;

    while (f.good()) //FIXME: another way to avoid errors?
    {
        char buffer[256];
        f.getline(buffer,255);
        bool IsHeader = false;

        for (int i = 0; i < 256; i++)
            if (buffer[i] == '|')
            {
                IsHeader = true;
                break;
            }
            else if (buffer[i] == NULL)
                break;
        if (IsHeader)
            continue;

        std::string tmp;
        bool IsValid = false;
        for (int i = 0; i < 256; i++)
        {
            if (buffer[i] != ':' && buffer[i] != ' ')
            {
                tmp += buffer[i];
            }
            else if (buffer[i] == ':')
            {
                IsValid = true;
                break;
            }
            if (buffer[i] == '\0' || buffer[i] == NULL || buffer[i] == '\n')
                break;
        }
        if (IsValid)
            interfaces.push_back(tmp);
    }

    if (DEBUG)
        fprintf(stderr,"Found %d interfaces\n",interfaces.size());

    return interfaces;
}*/

void RefreshWait()
{
    timespec TS = REFRESH;
    clock_nanosleep(CLOCK_MONOTONIC,0,&TS,NULL);
}

/*bool LoadDB(std::string Location, std::vector<TFSTATS> *DB)
{
    return true;
}

bool SaveDB(std::string Location, std::vector<TFSTATS> DB)
{
    //Save a rolling structure (oldest -> newest then overwrite oldest)
    //NEED TO HAVE TIME_ + long uint -> about 14 bytes per entry in dbase -> tells us the time and location of each entry;
    //should be ~7M of ram for 5 years of data taken every 5 mins;
    return true;
}*/
