#include "stdafx.h"
#include "PcscReader.h"
#include "PcscException.h"

PcscReader::PcscReader()
{
	sReaderName_ = "";
	pReaderStateList_ = NULL;
	uReaderStateListCount_ = 0;
	pPcscProvider_ = new PcscProvider();
}

PcscReader::PcscReader(PcscReader & rhs, string sReaderName)
{
	hContext_	= rhs.hContext_;
	sReaderName = sReaderName;
	pReaderStateList_ = NULL;
	uReaderStateListCount_ = 0;
}

PcscReader::~PcscReader()
{
	if (pReaderStateList_!= NULL)
	{
		uint8 uIdx;
		for (uIdx = 0; uIdx < uReaderStateListCount_; uIdx ++)
		{
			delete[] (int8*)(pReaderStateList_[uIdx].szReader);
		}
		delete[] pReaderStateList_;
	}
}

void PcscReader::initialize()
{
	int32 iResult;	
	
	iResult = SCardEstablishContext(SCARD_SCOPE_USER,NULL,NULL,&hContext_);
	if (iResult != SCARD_S_SUCCESS)
	{
		char *errorMsg;
		char aResult[512];

		errorMsg = pPcscProvider_->getSCardErrorMsg(iResult);
		sprintf(aResult, "[%ld] %s", iResult, errorMsg);
		throw PcscException(aResult);
	}
}

void PcscReader::cleanup()
{
	SCardReleaseContext(hContext_);
}

void PcscReader::listReaders(string *pReaders,
							 uint8  &rReaderCount)
{
	int32 iResult;
	DWORD cch = SCARD_AUTOALLOCATE;
	char *pReaderString  = NULL;
	char *pReaderStringBackup  = NULL;
	uint8 uActualReaderCount = 0;

	iResult = SCardListReaders(hContext_,NULL,
							   (LPTSTR)&pReaderString, &cch );
	if (iResult != SCARD_S_SUCCESS)
	{
		char *errorMsg;
		char aResult[512];

		errorMsg = pPcscProvider_->getSCardErrorMsg(iResult);
		sprintf(aResult, "[%ld] %s", iResult, errorMsg);
		throw PcscException(aResult);
	}	

	pReaderStringBackup = pReaderString;
	while ( '\0' != *pReaderString)
    {			
		pReaderString = pReaderString + strlen(pReaderString) + 1;		
		uActualReaderCount++;
    }

	if ( uActualReaderCount > rReaderCount)
	{
		throw PcscException("Insufficient Reader Count");
	}

	rReaderCount = 0;
	pReaderString = pReaderStringBackup;
	while ( '\0' != *pReaderString)
    {	
		pReaders[rReaderCount] = pReaderString;
		pReaderString = pReaderString + strlen(pReaderString) + 1;		
		rReaderCount++;
    }

	SCardFreeMemory(hContext_,pReaderStringBackup);
}


void PcscReader::listReaders(StringList &rReaderList)
{
	int32 iResult;
	DWORD uParameters = SCARD_AUTOALLOCATE;
	char *pReaderString  = NULL;
	char *pReaderStringBackup  = NULL;	

	iResult = SCardListReaders(hContext_,NULL,
							   (LPTSTR)&pReaderString, &uParameters );
	if (iResult != SCARD_S_SUCCESS)
	{
		char *errorMsg;
		char aResult[512];

		errorMsg = pPcscProvider_->getSCardErrorMsg(iResult);
		sprintf(aResult, "[%ld] %s", iResult, errorMsg);
		throw PcscException(aResult);
	}	

	pReaderStringBackup = pReaderString;

	rReaderList.clear();

	while ( '\0' != *pReaderString)
    {
		string sReaderElement = pReaderString;
		rReaderList.insert(rReaderList.end(),sReaderElement);
		pReaderString = pReaderString + strlen(pReaderString) + 1;		
    }	

	SCardFreeMemory(hContext_,pReaderStringBackup);
}

void PcscReader::listReaders(StringVector &rReaderList)
{
	int32 iResult;
	DWORD uParameters = SCARD_AUTOALLOCATE;
	char *pReaderString  = NULL;
	char *pReaderStringBackup  = NULL;	

	iResult = SCardListReaders(hContext_,NULL,
							   (LPTSTR)&pReaderString, &uParameters );
	if (iResult != SCARD_S_SUCCESS)
	{
		char *errorMsg;
		char aResult[512];

		errorMsg = pPcscProvider_->getSCardErrorMsg(iResult);
		sprintf(aResult, "[%ld] %s", iResult, errorMsg);
		throw PcscException(aResult);
	}	

	pReaderStringBackup = pReaderString;

	rReaderList.clear();

	while ( '\0' != *pReaderString)
    {
		string sReaderElement = pReaderString;
		rReaderList.insert(rReaderList.end(),sReaderElement);
		pReaderString = pReaderString + strlen(pReaderString) + 1;		
    }	

	SCardFreeMemory(hContext_,pReaderStringBackup);
}

void PcscReader::selectReader(void * pSelector)
{
	string sReaderName = *((string*)pSelector);
	this->sReaderName_ = sReaderName;
}

int32 PcscReader::connect()
{	
	if (sReaderName_ == "")
	{
		throw PcscException("Reader not set");
	}
	connect(sReaderName_);
	return 0;
}

int32 PcscReader::connect(string sReaderPort)
{
	int32 iResult;

	iResult = SCardConnect(hContext_,sReaderPort.data(),SCARD_SHARE_SHARED,
						   SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
						   &hCardHandle_, &uActiveProtocol_);
	if (iResult != SCARD_S_SUCCESS)
	{
		char *errorMsg;
		char aResult[512];

		errorMsg = pPcscProvider_->getSCardErrorMsg(iResult);
		sprintf(aResult, "[%ld] %s", iResult, errorMsg);
		throw PcscException(aResult);
	}
	return 0;
}

int32 PcscReader::connect(uint32 uSlotNumber)
{
	throw PcscException("Not supported for Pcsc");
}

int32 PcscReader::disconnect()
{
	SCardDisconnect(hContext_,SCARD_UNPOWER_CARD);
	return 0;
}

void PcscReader::registerReaderList(string *pReaderList, uint8 uReaderCount)
{
	uint8 uIdx;

	if (pReaderList == NULL || uReaderCount == 0)
	{
		throw PcscException("Invalid Parameters");
	}	

	SCARD_READERSTATE *pNewReaderStateList = new SCARD_READERSTATE[uReaderCount];
	if (pNewReaderStateList == NULL)
	{
		throw PcscException("Insufficient Memory.");
	}

	if (pReaderStateList_ != NULL)
	{
		for (uIdx = 0; uIdx < uReaderStateListCount_; uIdx ++)
		{
			delete[] (int8*)(pReaderStateList_[uIdx].szReader);
		}
		delete [] pReaderStateList_;
	}

	pReaderStateList_ = pNewReaderStateList;
	uReaderStateListCount_ = uReaderCount;

	memset(pReaderStateList_,0,sizeof(SCARD_READERSTATE) * uReaderCount);	

	for (uIdx = 0; uIdx < uReaderCount; uIdx ++)
	{
		uint8 uLen = strlen(pReaderList[uIdx].data()) + 1;
		pReaderStateList_[uIdx].szReader = new int8[uLen];

		//todo add checking here

		strcpy((int8*)pReaderStateList_[uIdx].szReader,pReaderList[uIdx].data());
	}
}

void PcscReader::getStatusChange(StatusChangeList & rStatusChangeList)
{
	if (pReaderStateList_ == NULL || uReaderStateListCount_ == 0 )
	{
		throw PcscException("No Readers Registered. Please call registerReaderList");
	}

	int32 iResult = SCardGetStatusChange(hContext_,INFINITE,pReaderStateList_,uReaderStateListCount_);

	if (iResult == SCARD_E_CANCELLED)
	{
		rStatusChangeList.clear();
		return;
	}
	else if (iResult != SCARD_S_SUCCESS)
	{
		throw PcscException("SCardGetStatusChange error");
	}

	uint8 uIdx;

	rStatusChangeList.clear();

	for(uIdx = 0; uIdx < uReaderStateListCount_; uIdx++)
	{
		if (ISBITSET(pReaderStateList_[uIdx].dwEventState,SCARD_STATE_CHANGED))
		{
			StatusChangeData cStatusChangeData;		

			cStatusChangeData.sReaderName = pReaderStateList_[uIdx].szReader;

			if (ISBITSET(pReaderStateList_[uIdx].dwEventState,SCARD_STATE_PRESENT) &&
				!ISBITSET(pReaderStateList_[uIdx].dwCurrentState,SCARD_STATE_PRESENT))
			{	
				cStatusChangeData.eStatus = CARD_STATUS_FOUND;
			}			
			else if(ISBITSET(pReaderStateList_[uIdx].dwEventState,SCARD_STATE_EMPTY) &&
					!ISBITSET(pReaderStateList_[uIdx].dwCurrentState,SCARD_STATE_EMPTY))
			{			
				cStatusChangeData.eStatus = CARD_STATUS_NOT_FOUND;
			}
			else
			{
				//todo: how to handle this?
				//cStatusChangeData.eStatus = CARD_STATUS_ERROR;
				break;
			}		

			pReaderStateList_[uIdx].dwCurrentState = pReaderStateList_[uIdx].dwEventState;

			rStatusChangeList.insert(rStatusChangeList.end(),cStatusChangeData);
		}		
	}
}

int32 PcscReader::getStatus()
{
	DWORD rdrLen = 100;
	int32 iResult;
	DWORD dwState;
	int8 aReaderName[255];
	uAtrLen_ = 255;


	iResult = SCardStatus(hCardHandle_, &aReaderName[0], &rdrLen, &dwState, &uActiveProtocol_, aAtr_, &uAtrLen_);

	if (iResult != SCARD_S_SUCCESS)
	{
		char *errorMsg;
		char aResult[512];

		errorMsg = pPcscProvider_->getSCardErrorMsg(iResult);
		sprintf(aResult, "[%ld] %s", iResult, errorMsg);
		throw PcscException(aResult);
	}

	return 0;
}

void PcscReader::cancel()
{
	SCardCancel(hContext_);
}

int32 PcscReader::getAtr(uint8 *pAtr, uint32 &rAtrLen)
{
	if (pAtr == NULL || rAtrLen < uAtrLen_)
	{
		throw PcscException("Invalid Parameters");
	}

	memcpy(pAtr,aAtr_,uAtrLen_);
	rAtrLen = uAtrLen_;
	return 0;
}

int32 PcscReader::sendApduCommand()
{
	uint32 uSendLen;
	uint32 uRecvLen;
	uint8* pRecvBuffer = NULL;
	uint8* pSendBuffer = NULL;
	Apdu* pApdu;

	pApdu = getApduCommand();	

	uSendLen = pApdu->getSendLen();

	pSendBuffer = new byte[uSendLen];
	if (pSendBuffer == NULL)
	{
		throw PcscException("Out of Memory");
	}
	
	pSendBuffer[0] = pApdu->getInstructionClass();
	pSendBuffer[1] = pApdu->getInstructionCode();
	pSendBuffer[2] = pApdu->getParameter1();
	pSendBuffer[3] = pApdu->getParameter2();
	pSendBuffer[4] = pApdu->getParameter3();
	
	if (pApdu->getSendData() != NULL)
		memcpy(pSendBuffer + 5, pApdu->getSendData(), uSendLen - 5);

	uRecvLen = pApdu->getReceiveLen();

	if (uRecvLen != 0)
	{
		pRecvBuffer = new byte[uRecvLen];
		if (pRecvBuffer == NULL)
		{
			delete [] pSendBuffer;
			throw PcscException("Out of Memory");
		}
	}	

	try
	{
		sendApduCommand(pSendBuffer,uSendLen,pRecvBuffer,uRecvLen);
	}
	catch (PcscException cException)
	{
		if (pSendBuffer != NULL)
		{
			delete [] pSendBuffer;
		}		
		if (pRecvBuffer != NULL)
		{
			delete [] pRecvBuffer;
		}
		throw cException;
	}

	pApdu->setReceiveLen(uRecvLen);
	memcpy(pApdu->getReceiveData(),pRecvBuffer,uRecvLen);
	
	if (pSendBuffer != NULL)
	{
		delete [] pSendBuffer;
	}	
	if (pRecvBuffer != NULL)
	{
		delete [] pRecvBuffer;
	}	

	return 0;
}

int32 PcscReader::sendApduCommand(Apdu* pApdu)
{
	setApduCommand(pApdu);

	return sendApduCommand();
}

int32 PcscReader::sendApduCommand(uint8 *	pCommand,
								  uint32		uCommandLen,
								  uint8 *	pResponse,
								  uint32 &	rResponseLen)
{
	SCARD_IO_REQUEST ioRequest;

	ioRequest.dwProtocol = uActiveProtocol_;
	ioRequest.cbPciLength = 8;

	int32 iResult;
	uint64 uActualResponseLength = rResponseLen;

	cSendCommandFunctor_(pCommand, uCommandLen);

	iResult = SCardTransmit(hCardHandle_, &ioRequest,
							pCommand, uCommandLen,
							NULL,pResponse, 
							&uActualResponseLength);
	if (iResult != SCARD_S_SUCCESS)
	{		
		char *errorMsg;
		char aResult[512];

		errorMsg = pPcscProvider_->getSCardErrorMsg(iResult);
		sprintf(aResult, "[%ld] %s", iResult, errorMsg);
		throw PcscException(aResult);
	}

	rResponseLen = uActualResponseLength;

	cReceiveCommandFunctor_(pResponse, rResponseLen);

	return 0;
}

int32 PcscReader::sendControlCommand(Apdu* pApdu)
{
	return 0;
}

int32 PcscReader::sendControlCommand(uint8  *pCommand,
									 uint32  uCommandLen,
									 uint8  *pResponse,
									 uint32 &rResponseLen)
{
	int32 iResult;
	DWORD uControlCode = 0; // TODO: provide value for this
	uint64 uActualResponseLen;

	iResult = SCardControl( hCardHandle_, uControlCode,
							pCommand, uCommandLen,
							pResponse,rResponseLen,
							&uActualResponseLen );	
	if ( iResult != SCARD_S_SUCCESS )
	{
		char *errorMsg;
		char aResult[512];

		errorMsg = pPcscProvider_->getSCardErrorMsg(iResult);
		sprintf(aResult, "[%ld] %s", iResult, errorMsg);
		throw PcscException(aResult);
	}
	return 0;
}

