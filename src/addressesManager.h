#ifndef _ADDRESSESMANAGER_H_
#define _ADDRESSESMANAGER_H_

#include "memoryManager.h"
#include "fileManager.h"

void processAddrFile(MemoryManager memMng, const char* fPathAddr, FILE* fSaida);

#endif