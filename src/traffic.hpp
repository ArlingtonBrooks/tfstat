#ifndef __TRAFFIC_HPP_
#define __TRAFFIC_HPP__

#include "Common.hpp"

#if defined(__linux__)
/*
GetState
Gets the current state of interface iface
Accepts: string name of interface being parsed
Returns: State of interface in STATS_ form (bits/packets rcv/xmit)
*/
STATS_ GetState(std::string iface);

/*
GetDeltaState
Gets the current state relative to the 'Last' state for interface
Accepts: Last state, interface name
Returns: change in state between last state and current state
*/
STATS_ GetDeltaState(STATS_ Last, std::string iface);

/*
InitIface
Initializes an interfaces for information tracking
Accepts: nothing
Returns: list of all interfaces (based on global interface list)
*/
std::vector<IFACE_STAT> InitIface();

/*
FindInterfaces
Creates a list of interfaces based on /proc/net/dev
Accepts: /proc/net/dev string
Returns: list of interfaces found in /proc/net/dev
*/
std::vector<std::string> FindInterfaces(std::string data); //REQ data=PROC_NET_DEV;

/*
ReadNetStat
Updates counter statistics in /proc/net/dev
Accepts: nothing
Returns: true if able to refresh;  false otherwise.
*/
bool ReadNetStat();
#elif defined(__freebsd__)

#endif

/*
FindInString
Searches for a string within another string
Accepts: null terminated string to search, null terminated string to find, whether they need to exactly match, list of characters (delim)
Returns: integer location in the stringtosearch where the stringtofind is located
*/
int FindInString(const char* stringtosearch, const char* stringtofind, bool ExactMatch, const char* ignores);

#endif
