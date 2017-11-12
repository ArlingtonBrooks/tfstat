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
#include "tfstat.hpp"
#include "traffic.hpp"
#include "defaults.hpp"
#include "Signals.cpp"
#include "Database.hpp"

#if defined(__linux__) || defined(__linux) || defined(linux)
PROCNET PROC_NET_DEV = {"",false};

/*
SetProcLock
*/
bool SetProcLock(bool state)
{
    int iter = 0;
    if (state == true)
        while (PROC_NET_DEV.LOCK && iter < 100000000)
            iter++;
    if (iter >= 100000000-1)
    {
        fprintf(stderr,"Failed to lock /proc/net/dev\n");
        return false;
    }
    PROC_NET_DEV.LOCK = state;
    return state;
}
#endif

int main (int argc, char** argv)
{
    DEBUG = true;
    ReadCFG(CFGFILE);
    ParseArgs(argc,argv);
    bool Running = true;
    std::vector<IFACE_STAT> Statlist;

    Statlist = InitIface(); //TODO: Pass IFaceList for init;
//    ReadNetStat();
//    GetState("enp2s0");

    if (OneShot)
        Running = false;

    SetExitHandler(SIGTERM);
    SetExitHandler(SIGINT);
    SetExitHandler(SIGQUIT);
    //DumpDatabase("tfstat.dbenp2s0.db");
    //DumpDatabase("tfstat.dblo.db");
    //DumpKeys("tfstat.dbenp2s0.tbl");
    
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
            {
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
    }
    printf("SAVED\n");
    return EXIT_SUCCESS;
    //NEED LIST OF INTERFACES TO SCAN FOR!
}

/*
 * Parse Options
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

/*
CheckStrValues
*/
bool CheckStrValues(const char* str, char val_low, char val_hi) //TODO: ->Common.hpp
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
void RefreshWait() //TODO: ->Common.hpp
{
    timespec TS = REFRESH;
    clock_nanosleep(CLOCK_MONOTONIC,0,&TS,NULL);
}

/*
ReadCFG
*/
void ReadCFG(const char* FNAME)
{
    std::fstream f;
    f.open(FNAME,std::ios::in);
    if (!f)
    {
        fprintf(stderr,"ERROR: Unable to load configuration at %s.  Continuing with default values...\n",FNAME);
        IFaceList.push_back("all");
        return;
    }

    while (f.good()) //TODO: reliability;
    {
        char C = f.get();
        //Skip comment lines;
        if (C == '#')
        {
            f.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            continue;
        }

        std::string PName, PParam;
        if (PParam.compare(" ") == 0)
            PParam = "";
        //Read parameter name:
        PName = GetLine(f,CfgDelim);
        PParam = GetLine(f,CfgDelim);

        if (PName.compare("REFRESH_TIME") == 0)
        {
            int sec,nsec;
            sscanf(PParam.c_str()," %d , %d ",&sec,&nsec);
            REFRESH = {sec,nsec};
        }
        else if (PName.compare("SAVE_INTERVAL") == 0)
        {
            int yy,mm,dd,hh,mim;
            sscanf(PParam.c_str()," %d , %d , %d , %d , %d ",&yy,&mm,&dd,&hh,&mim);
            Delta = {yy - 1900, mm, dd, hh, mim};
        }
        else if (PName.compare("SAVE_HISTORY") == 0)
        {
            int yy,mm,dd,hh,mim;
            sscanf(PParam.c_str()," %d , %d , %d , %d , %d ",&yy,&mm,&dd,&hh,&mim);
            History = {yy - 1900, mm, dd, hh, mim};
        }
        else if (PName.compare("BASE_DIR") == 0)
        {
            Base_Dir = PParam;
        }
        else if (PName.compare("USE_BITS") == 0)
        {
            if (PParam.compare("1") == 0)
                BitConvert = true;
            else if (PParam.compare("0") == 0)
                BitConvert = false;
            else
                printf("Problem reading parameter 'USE_BITS' = %s\n",PParam.c_str());
        }
        else if (PName.compare("SAVE_ALL") == 0)
        {
            if (PParam.compare("1") == 0)
                SaveAll = true;
            else if (PParam.compare("0") == 0)
                SaveAll = false;
            else
                printf("Problem reading parameter 'SAVE_ALL' = %s\n",PParam.c_str());
        }
        else if (PName.compare("IFACE_LIST") == 0)
        {
            if (PParam.compare("all") == 0 || PParam.compare("") == 0)
            {
                IFaceList.push_back("all");
            }
            else
            {
                //Read comma-separated interface names;
                IFaceList = ParseStrList(PParam,",");
            }
        }
        else if (PName.compare("COMPRESS_DBASE") == 0)
        {
            //Read DBase Compression (Not Implemented!)
            fprintf(stderr,"Database Compression not implemented: %s\n",PParam.c_str());
        }
        else if (PName.compare("CONFIG_FILE") == 0)
        {
            printf("Why are you re-specifying the LOCATION OF THE \\\\CONFIG FILE// *HERE*???\n");
        }
        else
        {
            fprintf(stderr,"Error: unable to parse config option '%s'\n",PName.c_str());
            OneShot = true;
            f.close();
            return;
        }
    }
    if (IFaceList.size() < 1)
        IFaceList.push_back("all");
    f.close();
}

/*
GetLine
*/
std::string GetLine(std::fstream& f,char* delim = NULL) //TODO: ->Common.hpp
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
