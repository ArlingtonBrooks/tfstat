#ifndef __SIGNALS_CPP_
#define __SIGNALS_CPP_
#include <signal.h>

bool IsExiting = false;

void term(int SigNum)
{
    IsExiting = true;
}

void SetExitHandler(int SigNum)
{
    struct sigaction act;
    act.sa_handler = term;
    sigaction(SigNum,&act,NULL);
}


#endif
