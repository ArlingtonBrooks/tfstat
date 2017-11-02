#ifndef __TRAFFIC_HPP_
#define __TRAFFIC_HPP__

#include "Common.hpp"

#if defined(__linux__)
STATS_ GetState(std::string iface);
STATS_ GetDeltaState(STATS_ Last, std::string iface);
std::vector<IFACE_STAT> InitIface();
std::vector<std::string> FindInterfaces(std::string data); //REQ data=PROC_NET_DEV;
bool ReadNetStat();
#elif defined(__freebsd__)

#endif

int FindInString(const char* stringtosearch, const char* stringtofind, bool ExactMatch, const char* ignores);

#endif
