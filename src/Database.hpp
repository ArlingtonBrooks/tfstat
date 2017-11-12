#ifndef __DATABASE_HPP_
#define __DATABASE_HPP_
#include "Common.hpp"
#include <algorithm>

//Yeah... a database isn't exactly needed...
//but if we want to, we can change how much/what we store in the database.
//With this structure, we only save about 60% of memory usage, which isn't much considering it's on the order of 20-40 MB
//TODO: I'll implement an SSD mode which uses fewer write cycles and no DBASE struct.
/*
Database key entry structure
Stores location in database file corresponding to TimeVal
*/
struct DBASE_ENTRY
{
    TIME_ TimeVal;
    unsigned int SeekPos;
};

/*
Boolean comparisons of database entries based on time
*/
bool operator<(DBASE_ENTRY a, const DBASE_ENTRY b);
bool operator>(DBASE_ENTRY a, const DBASE_ENTRY b);
bool operator<=(DBASE_ENTRY a, const DBASE_ENTRY b);
bool operator>=(DBASE_ENTRY a, const DBASE_ENTRY b);

/*
SortDBase
Performs a heapsort to organize database entries based on time
Accepts: pointer to database entry array, database array size, boolean indicating whether to sort from high to low
Returns: nothing (directly modifies database)
*/
void SortDBase(DBASE_ENTRY* DB, unsigned int size, bool HTL = false);

/*
LoadDB
Load database keys from location Loc
Accepts: Database name (Loc)
Returns: vector of database keys
*/
std::vector<DBASE_ENTRY> LoadDB(std::string Loc);

/*
SaveDBK
Saves database keys to Loc
Accepts: Database name, list of keys, boolean indicating whether to fail when history limit is hit
Returns: true if successful, false otherwise.
*/
bool SaveDBK(std::string Loc, std::vector<DBASE_ENTRY> vals, bool FailOnHistoryLimit = false);

/*
SaveToDB
Saves a stat entry to database
Accepts: Location of database, Location of keybase, Entry to be added, vector of database keys
Returns: List of database keys including newly added entry
*/
std::vector<DBASE_ENTRY> SaveToDB(std::string Location, std::string DBK_Location, TFSTATS Entry, std::vector<DBASE_ENTRY> dbk);

/*
checksum
Calculates a checksum of database file (useful for checking for corrupt data)
Accepts: Database location, amount of data to skip at start of file
Returns: checksum (sum of all bytes retrieved from file)
*/
long unsigned int checksum(std::string Location, unsigned int SkipStart = 0);

/*
Dump*
Dumps information stored in Database/Key files at Location
Accepts: Location of file to dump
Returns: nothing (prints directly to terminal)
*/
void DumpDatabase(std::string Location);
void DumpKeys(std::string Location);

#endif
