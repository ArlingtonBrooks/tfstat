#ifndef __SIGNALS_CPP_
#define __SIGNALS_CPP_
#include <signal.h>

bool IsExiting = false; //Global variable defining whether program is exiting

/*
term
Tells program to exit
Accepts: Exit signal (SigNum)
Returns: nothing (sets global IsExiting variable);
*/
void term(int SigNum)
{
    IsExiting = true;
}

/*
SetExitHandler
Sets exit action for SigNum
Accepts: Exit signal (SigNum)
Returns: nothing (sets 'term' as handle for signal SigNum)
*/
void SetExitHandler(int SigNum)
{
    struct sigaction act;
    act.sa_handler = term;
    sigaction(SigNum,&act,NULL);
}

#endif
