#include "stdafx.h"
#include "PcscReader.h"
#include "PcscException.h"
#include "Osa.h"


struct PcscPollingList
{
	PcscReader *pReader;
	SCARD_READERSTATE *pReaderStates;
	CString *pReaderStrings;	
	uint32 uNumReaders;
};

// Polling
uint32 PcscReader::getStatusChange(void * pParam)
{
	PcscPollingList * pPollingList = (PcscPollingList *) pParam;
	return SCardGetStatusChange(hContext_,
								INFINITE,
								pPollingList->pReaderStates,
								pPollingList->uNumReaders);
}

uint32 PcscReader::setPollingList(int8 ** ppReaderList, uint32 uNumReaders)
{
	return ACSTAT_OK;
}

uint32 PcscReader::startPolling(void *pReaderData, uint32 uNumReaders)
{
	PcscPollingList *pPollingList;
	SCARD_READERSTATE *pReaderStates;
	CString *pReaderStrings;
	CString *pTempReaderStrings;
	uint32 uIdx;
	uint32 uResult;

	// Check Parameters
	if (uNumReaders == 0 || pReaderData == NULL)
	{
		throw PcscException("Invalid Parameters");
	}

	if (pPollingCallback_ == NULL)
	{
		throw PcscException("Callback Not Registered");
	}

	//Allocate Resources
	pPollingList = new PcscPollingList;
	if (pPollingList == NULL)
	{		
		throw PcscException("Insufficient Memory");
	}

	pReaderStates = new SCARD_READERSTATE[uNumReaders];
	if (pReaderStates == NULL)
	{
		delete pPollingList;
		throw PcscException("Insufficient Memory");
	}

	pTempReaderStrings = new CString[uNumReaders];
	if (pTempReaderStrings == NULL)
	{
		delete [] pReaderStates;
		delete pPollingList;
		throw PcscException("Insufficient Memory");
	}

	// Initialize Polling List Parameter
	pReaderStrings = (CString *) pReaderData;

	for (uIdx = 0; uIdx < uNumReaders; uIdx++)
	{	
		memset(pReaderStates + uIdx,0,sizeof(SCARD_READERSTATE));
		pTempReaderStrings[uIdx] = pReaderStrings[uIdx];
		pReaderStates[uIdx].szReader = pTempReaderStrings[uIdx].GetBuffer(100);
	}
	
	pPollingList->pReader = this;
	pPollingList->pReaderStates = pReaderStates;
	pPollingList->uNumReaders = uNumReaders;
	pPollingList->pReaderStrings = pTempReaderStrings;

	// Start Thread
	uResult = OSA_BeginThread(PcscReader::MainPollingThread,pPollingList);
	if (uResult != ACSTAT_OK)
	{
		throw PcscException("Failed to Begin Thread");
	}
	
	return ACSTAT_OK;
}

uint32 PcscReader::stopPolling()
{
	return SCardCancel(hContext_);
}

uint32 PcscReader::setPollingCallback(PcscPollingCallback pCallback)
{
	pPollingCallback_ = pCallback;
	
	return ACSTAT_OK;
}



UINT PcscReader::MainPollingThread( LPVOID pParam )
{
	uint32 uReturnCode;
	PcscPollingList *pPollingList;	
	PcscReader *pReader;
	SCARD_READERSTATE *pReaderStates;
	uint32 uIdx;

	pPollingList = (PcscPollingList *) pParam;
	pReader = pPollingList->pReader;
	pReaderStates = pPollingList->pReaderStates;

	//Read Intial Status of each Reader
	uReturnCode = pReader->getStatusChange(pPollingList);		
	if (uReturnCode != SCARD_S_SUCCESS)
	{	
		pReader->pPollingCallback_(POLL_STATUS_ERROR,
								(void*)pReaderStates[0].szReader);
		delete [] pPollingList->pReaderStrings;
		delete [] pReaderStates;
		delete pPollingList;
		return ACSTAT_OK;
	}

	for (uIdx = 0; uIdx < pPollingList->uNumReaders; uIdx++)
	{
		pReaderStates[uIdx].dwCurrentState = pReaderStates[uIdx].dwEventState;

		if (ISBITSET(pReaderStates[uIdx].dwEventState,SCARD_STATE_EMPTY))
		{
			pReader->pPollingCallback_(POLL_STATUS_NOT_FOUND,
						(void*)pReaderStates[uIdx].szReader);
		}
		if (ISBITSET(pReaderStates[uIdx].dwEventState,SCARD_STATE_PRESENT))
		{
			pReader->pPollingCallback_(POLL_STATUS_FOUND,
						(void*)pReaderStates[uIdx].szReader);
		}
	}

	while(1)
	{
		uReturnCode = pReader->getStatusChange(pPollingList);
		
		if (uReturnCode != SCARD_S_SUCCESS)
		{	
			if (uReturnCode != SCARD_E_CANCELLED)
			{
				pReader->pPollingCallback_(POLL_STATUS_ERROR,
								NULL);
			}
			break;
		}

		for (uIdx = 0; uIdx < pPollingList->uNumReaders; uIdx++)
		{
			if (ISBITSET(pReaderStates[uIdx].dwEventState,SCARD_STATE_CHANGED))
			{				
				//if card readers are plugged or unplugged
				if (OSA_Memcmp((void*)pReaderStates[uIdx].szReader,"\\\\?PnP?\\Notification",20) == 0)
				{	
					uint32 uCurrentReaderCount;
					uint32 uNewReaderCount;

					uNewReaderCount		= GET32MSWORD(pReaderStates[uIdx].dwEventState);
					uCurrentReaderCount = GET32MSWORD(pReaderStates[uIdx].dwCurrentState);

					if (uNewReaderCount > uCurrentReaderCount)
					{
						pReader->pPollingCallback_(POLL_STATUS_NEW_READER_FOUND, NULL);
					}
					else
					{
						pReader->pPollingCallback_(POLL_STATUS_READER_REMOVED, NULL);
					}					
				}
				else // any other reader
				{
					if (ISBITSET(pReaderStates[uIdx].dwEventState,SCARD_STATE_EMPTY))
					{
						pReader->pPollingCallback_(POLL_STATUS_NOT_FOUND,
									(void*)pReaderStates[uIdx].szReader);
					}
					if (ISBITSET(pReaderStates[uIdx].dwEventState,SCARD_STATE_PRESENT))
					{
						pReader->pPollingCallback_(POLL_STATUS_FOUND,
									(void*)pReaderStates[uIdx].szReader);
					}
				}

				pReaderStates[uIdx].dwCurrentState = pReaderStates[uIdx].dwEventState;
			}
		}
	}

	//cleanup resources
	delete [] pPollingList->pReaderStrings;
	delete [] pReaderStates;
	delete pPollingList;
	
	return ACSTAT_OK;

}