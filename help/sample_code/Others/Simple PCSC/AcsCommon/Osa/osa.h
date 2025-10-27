#pragma once
#include "ACSCommon.h"

typedef unsigned int (*ThreadFunction) (void * pParam);

void OSA_Memcpy(void * pDest, void * pSource, uint32 uSize);
int32 OSA_Memcmp(void * pDest, void * pSource, uint32 uSize);
int32 OSA_BeginThread(ThreadFunction pThreadFunction, void * pParameters);