#ifndef ACCEPT_ANALYZE_H
#define ACCEPT_ANALYZE_H

#include "WifiTask.h"

extern int saveHttpDataAndanalyze(char FrontTcpLinkID);
extern void deleteCmdTask(void);
extern int getCmdTaskNumberOrLinkID(int UserCase);
#endif
