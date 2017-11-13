#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Common.hpp"
#include "tfstat.hpp"
#include "traffic.hpp"

#if defined(__linux__) || defined(__linux) || defined(linux)
//Pull Data from /proc/net/dev

//Takes data as a line from /proc/net/dev;
STATS_ GetState(std::string iface)
{
    STATS_ ret;
    
    SetProcLock(true);

    std::istringstream ss(PROC_NET_DEV.str);
    char buffer[256];
    ss.getline(buffer,255);

    int pos = -1;

    while (ss.good()) //FIXME: another way to avoid errors?
    {
        pos = FindInString(buffer,iface.c_str(),true,":");
        if (pos == -1)
            ss.getline(buffer,255);
        else
            break;
    }

    if (pos < 0)
    {
        printf("Invalid interface data passed: %s\n",iface.c_str());
        return ret;
    }
    //Assumes standard /proc/net/dev format
    sscanf(&buffer[pos+1]," %lu %lu %*d %*d %*d %*d %*d %*d %lu %lu %*d %*d %*d %*d %*d %*d ",&ret.b_rcv,&ret.pk_rcv,&ret.b_tx,&ret.pk_tx);
    printf("DEBUG: Buffer Read %lu, %lu, %lu, %lu\n",ret.b_rcv,ret.pk_rcv,ret.b_tx,ret.pk_tx);
    SetProcLock(false);
    return ret;
}

STATS_ GetDeltaState(STATS_ Last, std::string iface)
{
    STATS_ ret;
    
    SetProcLock(true);

    std::istringstream ss(PROC_NET_DEV.str);
    char buffer[256];
    ss.getline(buffer,255);

    int pos = -1;

    while (ss.good())//FIXME: another way to avoid errors?
    {
        pos = FindInString(buffer,iface.c_str(),true,":");
        if (pos == -1)
            ss.getline(buffer,255);
        else
            break;
    }

    if (pos < 0)
    {
        printf("Invalid interface data passed: %s\n",iface.c_str());
        return ret;
    }
printf("BUFFER: %s\n",buffer);

    //Assumes standard /proc/net/dev format
    sscanf(&buffer[pos+1]," %lu %lu %*d %*d %*d %*d %*d %*d %lu %lu %*d %*d %*d %*d %*d %*d ",&ret.b_rcv,&ret.pk_rcv,&ret.b_tx,&ret.pk_tx);
    SetProcLock(false);
printf("%d TST\n",ret.b_rcv);
    return ret - Last;
}

std::vector<IFACE_STAT> InitIface() //TODO: this needs to init ONLY the desired interfaces;
{
    std::vector<IFACE_STAT> ret;
    
    ReadNetStat();

    //Get list of available interfaces;
    std::vector<std::string> IfaceNames = FindInterfaces(PROC_NET_DEV.str);
    TIME_ tm = GetTimeNow();
    //std::vector<TFSTATS> st;
    for (int i = 0; i < IfaceNames.size(); i++)
    {
        bool IsValid = false;
        for (int j = 0; j < IFaceList.size(); j++)
        {
            if (IFaceList[j].compare(IfaceNames[i]) == 0 || IFaceList[j].compare("all") == 0)
            {
                IsValid = true;
                break;
            }
        }
        STATS_ tmp = GetState(IfaceNames[i]); //List of interfaces
        TFSTATS tmpst; 
        IFACE_STAT tmpstat; 

        tmpst.Statlist = tmp;
        tmpst.DateTimeZulu = tm;

        tmpstat.Data = tmpst;
        tmpstat.Iface = IfaceNames[i];
        tmpstat.Active = IsValid;

        ret.push_back(tmpstat);
    }

    if (ret.size() > IFaceList.size() && IFaceList[0].compare("all") != 0)
    {
        for (int i = 0; i < IFaceList.size(); i++)
        {
            for (int j = 0; j < ret.size(); j++)
            {
                if (ret[j].Iface.compare(IFaceList[i]) == 0)
                {
                    break;
                }
                if (j == ret.size() - 1)
                    fprintf(stderr,"WARNING: Unable to open interface '%s'\n",IFaceList[i].c_str());
            }
        }
        //Figure out which which list item is missing
        //Print error re: interface not found;
    }

    /*for (int i = 0; i < st.size(); i++)
    {
        IFACE_STAT tmp;
        tmp.Data = st[i];
        tmp.Iface = IfaceNames[i];
        ret.push_back(tmp);
    }*/
    return ret;
}

bool ReadNetStat()
{
    if (DEBUG)
        fprintf(stderr,"Reading network status\n");
    std::fstream f;
    f.open("/proc/net/dev",std::fstream::in);
    if (!f)
    {
        fprintf(stderr,"Failed to load /proc/net/dev\n");
        return false;
    }

    SetProcLock(true);
    PROC_NET_DEV.str.clear();

    while (f.good()) //FIXME: another way to avoid errors?
    {
//        char ii = f.get();
//        printf("%c",ii);
        PROC_NET_DEV.str += f.get();
        if (PROC_NET_DEV.str.back() == EOF)
            PROC_NET_DEV.str.pop_back();
    }
    SetProcLock(false);

    if (DEBUG)
        fprintf(stderr,"Read network status (%d): %s \n",PROC_NET_DEV.str.length(),PROC_NET_DEV.str.c_str());

    f.close();

    return true;
}

std::vector<std::string> FindInterfaces(std::string data)
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
            if (buffer[i] != ':' && buffer[i] != ' ' && buffer[i] != '\0')
            {   
                tmp += buffer[i];
            }
            else if (buffer[i] == ':')
            {   
                IsValid = true;
                break;
            }
            if (buffer[i] == '\0' || buffer[i] == '\n' || buffer[i] == EOF)
                break;
        }
        if (IsValid)
            interfaces.push_back(tmp);
    }

    if (DEBUG)
        fprintf(stderr,"Found %d interfaces\n",interfaces.size());

    return interfaces;
}
#elif defined(__freebsd__)

#endif
