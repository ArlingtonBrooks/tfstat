#ifndef __DEFAULTS_HPP__
#define __DEFAULTS_HPP__
#include "tfstat.hpp"
//SET THIS UP FOR OBJ FILE!

//Global default variables:
timespec REFRESH = {10,0};//seconds between refresh - should be an integer division of Delta;  Lower is better, but must be more than 2 seconds.
TIME_ Delta = {0,0,0,0,2}; //Length of time before commiting a value to database.
TIME_ History = {0,0,0,1,0}; //Length of time to keep any given value;  Y/M/D/H/Mi

#define CFGFILE = "/etc/tfstat.conf"
//MAKE THIS 1 FILE PER INTERFACE!
std::string Base_Dir = "./";
std::string Base_DBase_Loc = "tfstat.db"; //"/var/db/tfstat.db"; //actual database
std::string DBase_Ext = ".db";
std::string DBase_Key_Ext = ".tbl";

bool BitConvert = false; //convert from bytes to bits;
bool OneShot = false;

#endif
