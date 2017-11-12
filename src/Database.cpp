#include "Common.hpp"
#include "Database.hpp"

bool operator<(DBASE_ENTRY a, const DBASE_ENTRY b) {return (a.TimeVal < b.TimeVal);};
bool operator>(DBASE_ENTRY a, const DBASE_ENTRY b) {return (a.TimeVal < b.TimeVal);};
bool operator<=(DBASE_ENTRY a, const DBASE_ENTRY b) {return (a.TimeVal < b.TimeVal);};
bool operator>=(DBASE_ENTRY a, const DBASE_ENTRY b) {return (a.TimeVal < b.TimeVal);};

//FIXME: doesn't appear to be properly overwriting old DBase entries.
//First 64-bits is saved checksum
std::vector<DBASE_ENTRY> LoadDB(std::string KeyLoc)
{
    long unsigned int chk = checksum(KeyLoc,sizeof(long unsigned int));
    if (DEBUG)
        fprintf(stderr,"Database Keylist Checksum: %lu\n",chk);

    std::vector<DBASE_ENTRY> DBK;
    
    std::fstream f;
    f.open(KeyLoc.c_str(),std::ios::in | std::ios::binary);
    if (!f)
        return DBK;
    long unsigned int chk_file;
    f.read((char*)&chk_file,sizeof(long unsigned int));
    if (chk_file != chk)
    {
        fprintf(stderr,"Checksum is invalid for %s: %lu != %lu\n",KeyLoc.c_str(),chk_file,chk);
        return DBK;
    }
    while (f.good()) //FIXME: another way to avoid errors?
    {
        DBASE_ENTRY et;
        f.read((char*)&et,sizeof(DBASE_ENTRY));
        if (f.gcount() != sizeof(DBASE_ENTRY))
        {
            printf("Unable to read entry from database %s\n",KeyLoc.c_str());
            break;
        }
        else
            DBK.push_back(et);
    }
    if (DEBUG)
        fprintf(stderr,"Database keylist loaded successfully\n");

    f.close();

    SortDBase(&DBK[0],DBK.size());
    return DBK;
}

long unsigned int checksum(std::string FileLoc, unsigned int SkipStart)
{
    std::fstream f;
    long unsigned int SUM = 0;
    unsigned char VAL = 0;
    f.open(FileLoc.c_str(),std::ios::in | std::ios::binary);
    f.seekp(SkipStart);
    while (f.good()) //FIXME: another way to avoid errors?
    {
        f.read((char*)&VAL,sizeof(char));
        if (f.gcount() == sizeof(char))
            SUM += (long unsigned int) VAL;
    }
    f.close();
    return SUM;
}

void SortDBase(DBASE_ENTRY* DB, unsigned int size, bool HTL)
{
    //Create a min heap (algorithm)
    std::make_heap(DB,DB+size);
    //Sort min heap (algorithm)
    std::sort_heap(DB,DB+size);
    //Reverse if desired;
    if (HTL)
        std::reverse(DB,DB+size);
}

//FIXME: check for errors; //FIXME: this might multiply number of keys between uses.
bool SaveDBK(std::string Location, std::vector<DBASE_ENTRY> vals, bool FailOnHistoryLimit)
{
    if (vals.size() > 0)
        SortDBase(&vals[0],vals.size());
    else
    {
        fprintf(stderr,"ERROR: no database values to save.\n");
        return 0;
    }

    if (FailOnHistoryLimit && vals.back().TimeVal - vals.front().TimeVal > History)
    {
        fprintf(stderr,"Exceeds history limit!\n");
        return 0;
    }
    if (vals.back().TimeVal - vals.front().TimeVal > History)
        fprintf(stderr,"WARNING: History Limit Exceeded.  Saving anyway...");

    std::fstream f;
    f.open(Location.c_str(),std::ios::in|std::ios::out|std::ios::binary);
    if (!f)
    {
        MakeFile(Location);
        f.open(Location.c_str(),std::ios::in|std::ios::out|std::ios::binary);
    }
    f.seekp(sizeof(long unsigned int)); //skip checksum storage;
    for (int i = 0; i < vals.size(); i++)
    {
        f.write((char*)&vals[i],sizeof(DBASE_ENTRY));
    }

    //save checksum
    f.flush();
    long unsigned int CK = checksum(Location,sizeof(long unsigned int));
    f.seekp(0);
    f.write((char*)&CK,sizeof(long unsigned int));
    f.flush();
    f.close();
    printf("WROTE DBASE KEYS; CHKSUM = %d\n",CK);
    return true;
}

//FIXME: do a case for time1 == time2 and also FIGURE OUT WHY IT'S SAVING SO MANY!
std::vector<DBASE_ENTRY> SaveToDB(std::string Location, std::string DBK_Location, TFSTATS entry, std::vector<DBASE_ENTRY> dbk)
{
    std::fstream f;
    f.open(Location.c_str(),std::ios::in|std::ios::out|std::ios::binary|std::ios::ate);
    if (!f)
    {
        if (!MakeFile(Location))
            fprintf(stderr,"Failed to create file at %s\n",Location.c_str());
        f.open(Location.c_str(),std::ios::in|std::ios::out|std::ios::binary|std::ios::ate);
    }

    if (dbk.size() > 0)
    {
        SortDBase(&dbk[0],dbk.size(),true);
        unsigned int EndLoc = f.tellg();
        
        if (dbk.back().TimeVal - dbk.front().TimeVal > History)
        {//FIXME This doesn't actually work;
            printf("HISTORY EXCEEDS\n");
            //Overwrite oldest entry;
            if (entry.DateTimeZulu != dbk.back().TimeVal) //NB: Refresh time is handled externally;
            {//Check for overlap and write if necessary;
                TFSTATS LastVal;
                f.seekp(dbk.back().SeekPos);
                f.read((char*)&LastVal,sizeof(TFSTATS));
                printf("Last: %ld,%ld,%ld,%ld\n",LastVal.Statlist.b_rcv,LastVal.Statlist.pk_rcv,LastVal.Statlist.b_tx,LastVal.Statlist.pk_tx);
                printf("NEW: %ld,%ld,%ld,%ld\n",entry.Statlist.b_rcv,entry.Statlist.pk_rcv,entry.Statlist.b_tx,entry.Statlist.pk_tx);
                
                if (SaveAll || LastVal.Statlist != entry.Statlist)
                {
                    if (DEBUG)
                        printf("NEW DATA: Saving to file\n");
                    dbk[0].TimeVal = entry.DateTimeZulu; //Overwrite key time loc
        
                    f.seekp(dbk[0].SeekPos); //Overwrite dbase entry
                    f.write((char*)&entry,sizeof(TFSTATS));
                }
                else if (DEBUG)
                    printf("DEBUG: Not saving history due to stat overlap\n");
            }
            else
                if (DEBUG)
                    printf("DEBUG: Not saving history due to time overlap.\n");

            //dbk[0] = NewVal;

            //f.seekp(NewVal.SeekPos);
            //f.write((char*)&entry,sizeof(TFSTATS));
        }
        else
        {
            printf("History not exceeded\n");
            if (entry.DateTimeZulu != dbk.back().TimeVal)
            {
                TFSTATS LastVal;
                f.seekp(dbk.back().SeekPos);
                f.read((char*)&LastVal,sizeof(TFSTATS));
                printf("Last: %ld,%ld,%ld,%ld\n",LastVal.Statlist.b_rcv,LastVal.Statlist.pk_rcv,LastVal.Statlist.b_tx,LastVal.Statlist.pk_tx);
                printf("NEW: %ld,%ld,%ld,%ld\n",entry.Statlist.b_rcv,entry.Statlist.pk_rcv,entry.Statlist.b_tx,entry.Statlist.pk_tx);
                
                if (SaveAll || LastVal.Statlist != entry.Statlist)
                {
                    if (DEBUG)
                        printf("NEW DATA: Saving to file\n");
                    DBASE_ENTRY NewVal;
                    NewVal.TimeVal = entry.DateTimeZulu;
                    NewVal.SeekPos = EndLoc;
                    dbk.push_back(NewVal);
        
                    f.seekp(NewVal.SeekPos);
                    f.write((char*)&entry,sizeof(TFSTATS));
                }
                else if (DEBUG)
                    printf("DEBUG: Not saving history due to stat overlap\n");
            }
            else
                if (DEBUG)
                    printf("DEBUG: Not saving history due to time overlap.\n");
        }
        SaveDBK(DBK_Location,dbk,false);
    }
    else
    {
        if (DEBUG)
            printf("Database key is null length;  Saving to file;\n");
        DBASE_ENTRY NewVal;
        NewVal.TimeVal = entry.DateTimeZulu;
        NewVal.SeekPos = (unsigned int)f.tellg() + sizeof(long unsigned int);
        dbk.push_back(NewVal);

        f.seekp(NewVal.SeekPos);
        f.write((char*)&entry,sizeof(TFSTATS));
        SaveDBK(DBK_Location,dbk,false);
    }

    f.flush();
    f.close();

    return dbk;
}

void DumpDatabase(std::string Location)
{
    std::fstream f;
    f.open(Location.c_str(),std::ios::in|std::ios::binary);
    if (f.fail())
    {
        printf("Failed to open database file %s\n",Location.c_str());
        return;
    }
    printf("Database for %s\n",Location.c_str());
    printf("YYYYMMDDHHMM,Bytes RCV,Bytes TX,Packets RCV,Packets TX\n");
    f.seekp(sizeof(long unsigned int));
    TFSTATS Entry;
    while (f.good()) //FIXME: prints double at end of file;
    {
        f.read((char*)&Entry,sizeof(Entry));
        printf("%04d%02d%02d%02d%02d,",Entry.DateTimeZulu.year+1900, Entry.DateTimeZulu.month, Entry.DateTimeZulu.day, Entry.DateTimeZulu.hour, Entry.DateTimeZulu.minute); //Date
        printf("%lu,%lu,",Entry.Statlist.b_rcv,Entry.Statlist.b_tx); //Bytes
        printf("%lu,%lu\n",Entry.Statlist.pk_rcv,Entry.Statlist.pk_tx); //Packet count;
    }
}

void DumpKeys(std::string Location)
{
    std::fstream f;
    f.open(Location.c_str(),std::ios::in|std::ios::binary);
    f.seekp(sizeof(long unsigned int));
    DBASE_ENTRY DBE;
    while (f.good()) //FIXME: prints double at end of file;
    {
        f.read((char*)&DBE,sizeof(DBASE_ENTRY));
        printf("KEY: %lu at %02d:%02d on %d\n",DBE.SeekPos,DBE.TimeVal.hour,DBE.TimeVal.minute,DBE.TimeVal.day);
    }
}
