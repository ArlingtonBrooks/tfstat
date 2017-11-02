#ifndef __DATABASE_HPP_
#define __DATABASE_HPP_
#include "Common.hpp"
#include <algorithm>

//Yeah... a database isn't exactly needed...
//but if we want to, we can change how much/what we store in the database.
//With this structure, we only save about 60% of memory usage, which isn't much considering it's on the order of 20-40 MB
//TODO: I'll implement an SSD mode which uses fewer write cycles and no DBASE struct.
struct DBASE_ENTRY
{
    TIME_ TimeVal;
    unsigned int SeekPos;
};

bool operator<(DBASE_ENTRY a, const DBASE_ENTRY b);
bool operator>(DBASE_ENTRY a, const DBASE_ENTRY b);
bool operator<=(DBASE_ENTRY a, const DBASE_ENTRY b);
bool operator>=(DBASE_ENTRY a, const DBASE_ENTRY b);

void SortDBase(DBASE_ENTRY* DB, unsigned int size, bool HTL = false);
std::vector<DBASE_ENTRY> LoadDB(std::string Loc);
bool SaveDBK(std::string Loc, std::vector<DBASE_ENTRY> vals, bool FailOnHistoryLimit = false);
std::vector<DBASE_ENTRY> SaveToDB(std::string Location, std::string DBK_Location, TFSTATS Entry, std::vector<DBASE_ENTRY> dbk);
long unsigned int checksum(std::string Location, unsigned int SkipStart = 0);

void DumpDatabase(std::string Location);
void DumpKeys(std::string Location);

#endif
