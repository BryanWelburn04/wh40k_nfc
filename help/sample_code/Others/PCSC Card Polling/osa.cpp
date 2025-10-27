#include "stdafx.h"
#include "osa.h"
#include <memory.h>

void OSA_Memcpy(void * pDest, void * pSource, uint32 uSize)
{
	memcpy(pDest, pSource, uSize);
}

int32 OSA_Memcmp(void * pDest, void * pSource, uint32 uSize)
{
	return memcmp(pDest, pSource, uSize);
}

int32 OSA_BeginThread(ThreadFunction pThreadFunction, void * pParameters)
{
	CWinThread *pResult;
	
	pResult = AfxBeginThread(pThreadFunction, pParameters, 
							 THREAD_PRIORITY_NORMAL, 0,0, NULL);

	if (pResult == NULL)
	{
		return ACSTAT_FAIL;
	}

	return ACSTAT_OK;
}