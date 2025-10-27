//===========================================================================================
// 
//  Author          : Brey Vaughn Lee
// 
//  File            : PcscReader.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Contains Methods and API Wrappers for class PCSC
// 
//  Date            : February 1, 2013
// 
//  Revision Trail  : [Author] / [Date of modification] / [Details of Modifications done]
// 
// 
//=========================================================================================

#include "stdafx.h"
#include "PcscReader.h"
#include "PcscException.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PcscReader::PcscReader()
{	
	PcscReader("");	
}

PcscReader::PcscReader(int8 * pPortName)
{	
	memcpy(aReaderPort_,pPortName, sizeof(aReaderPort_));
	//sReaderPort_ = pPortName;
	hContext_ = 0;
}

long PcscReader::initialize()
{	
	uReturnCode_ = SCardEstablishContext(SCARD_SCOPE_USER ,NULL ,NULL ,&hContext_);	

	if (uReturnCode_ != SCARD_S_SUCCESS )
	{		
		throw PcscException(uReturnCode_);
	}

	return getReturnCode();	
}

void PcscReader::cleanup()
{
	SCardReleaseContext(hContext_);
	hContext_ = 0;	
}


//////////////////////////////////////////////////////////////////////
// Connect/Disconnect
//////////////////////////////////////////////////////////////////////

long PcscReader::connect(void *pReaderPort)
{
	int8 * pPortName;

	pPortName = (int8 *) pReaderPort;

	uReturnCode_ = SCardConnect(hContext_,
								pPortName,
								SCARD_SHARE_SHARED,
								SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
								&hCardHandle_,
								&uActiveProtocol_);
	if (uReturnCode_ != SCARD_S_SUCCESS)
	{
		throw PcscException(uReturnCode_);
	}

	tIoRequest_.dwProtocol = uActiveProtocol_;
	tIoRequest_.cbPciLength = sizeof (SCARD_IO_REQUEST);

	bConnectionActive_ = true;

	return uReturnCode_;	
}

long PcscReader::connect()
{
	if (pCurrentReader_ == NULL)
	{
		throw PcscException("No Reader Selected");
	}

	return connect(pCurrentReader_->getNameBuffer());
}

long PcscReader::disconnect()
{
	uReturnCode_ = SCardDisconnect(hCardHandle_,SCARD_LEAVE_CARD);

	bConnectionActive_ = false;
	uAtrLen_ = CFG_ATR_BUFFER_LEN;

	return getReturnCode();
}

long PcscReader::getAtr(uint8 *pAtr, uint32 &rAtrLen)
{
	int8 aReader[255];
	uint32 uReaderLen = 255;	

	if (pAtr == NULL || rAtrLen < uAtrLen_)
	{
		throw PcscException("Invalid Parameters");
	}	

	uReturnCode_ = SCardStatus( hCardHandle_,
								aReader,
								&uReaderLen,
								&uCurrentState_,
								&uActiveProtocol_,
								pAtr,
								&rAtrLen );

	if( uReturnCode_ != SCARD_S_SUCCESS )
	{	
		throw PcscException(uReturnCode_);
	}	

	return ACSTAT_OK;
}


//////////////////////////////////////////////////////////////////////
// Send/Receive PCSC specific commands
//////////////////////////////////////////////////////////////////////

long PcscReader::sendControlCommand()
{	
	return getReturnCode();
}

long PcscReader::sendControlCommand(uint8 *pCommand,
								 uint32 uCommandLen,
								 uint8 *pResponse,
								 uint32 *pResponseLen)
{
	uint32 uActualResponseLen;

	cSendCommandFunctor_(pCommand, uCommandLen);

	uReturnCode_ = SCardControl(hCardHandle_,getOperationControlCode(),
								pCommand, uCommandLen, pResponse, 
								*pResponseLen, &uActualResponseLen);	
	
	if (uReturnCode_ != SCARD_S_SUCCESS)
	{
		throw PcscException(uReturnCode_);
	}

	cReceiveCommandFunctor_(pResponse, uActualResponseLen);

	*pResponseLen = uActualResponseLen;

	return getReturnCode();
}


long PcscReader::sendControlCommand(Apdu *apdu)
{
	setApduCommand(apdu);

	return sendControlCommand();
}

//////////////////////////////////////////////////////////////////////
// Send/Receive Card-specific APDUs
//////////////////////////////////////////////////////////////////////

long PcscReader::sendApduCommand()
{
	uint32 uSendLen, uRecvLen;
	uint8* pRecvBuffer;
	uint8* pSendBuffer;
	Apdu* pApdu;

	pApdu = getApduCommand();	

	/*Prepare Send Data*/
	uSendLen = pApdu->getSendLen();

	pSendBuffer = new uint8[uSendLen];
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
	{
		memcpy(pSendBuffer + 5, pApdu->getSendData(), uSendLen - 5);
	}

	/*Prepare Receive Data*/
	uRecvLen = pApdu->getReceiveLen();
	pRecvBuffer = new uint8[uRecvLen];
	if (pRecvBuffer == NULL)
	{
		delete [] pSendBuffer;
		throw PcscException("Out of Memory");
	}
	
	cSendCommandFunctor_(pSendBuffer, uSendLen);

	
	uReturnCode_ = SCardTransmit(hCardHandle_,
                        &tIoRequest_,
                        pSendBuffer,
                        uSendLen,
                        NULL,
                        pRecvBuffer,
                        &uRecvLen );
	if ( uReturnCode_ != SCARD_S_SUCCESS)
	{
		delete [] pSendBuffer;
		delete [] pRecvBuffer;
		throw PcscException(uReturnCode_);
	}

	
	cReceiveCommandFunctor_(pRecvBuffer, uRecvLen);

	pApdu->setReceiveLen(uRecvLen);
	memcpy(pApdu->getReceiveData(),pRecvBuffer,uRecvLen);

	delete [] pSendBuffer;
	delete [] pRecvBuffer;

	return getReturnCode();
}


long PcscReader::sendApduCommand(uint8 * pCommand,
								 uint32 uCommandLen,
								 uint8 * pResponse,
								 uint32 &rResponseLen)
{	
	cSendCommandFunctor_(pCommand, uCommandLen);	

	uReturnCode_ = SCardTransmit(hCardHandle_,
                        &tIoRequest_,
                        pCommand,
                        uCommandLen,
                        NULL,
                        pResponse,
                        &rResponseLen );
	if (uReturnCode_ != ERROR_SUCCESS)
	{
		throw PcscException(uReturnCode_);
	}		
	
	cReceiveCommandFunctor_(pResponse, rResponseLen);
	
	return getReturnCode();
}

long PcscReader::sendApduCommand(Apdu *apdu)
{
	setApduCommand(apdu);

	return sendApduCommand();
}



uint32 PcscReader::listReaders(PcscReaderName ** ppReaderList, uint32 & rNumReaders)
{
	uint32 uBufferLen;
	int8 * pTempReaderList;	

	uReturnCode_ = SCardListReaders(hContext_,NULL,NULL,&uBufferLen);
	if (uReturnCode_ != SCARD_S_SUCCESS )
	{		
		throw PcscException(uReturnCode_);
	}

	pTempReaderList = new int8[uBufferLen];

	if (pTempReaderList == NULL)
	{
		throw PcscException("Insufficient Memory");
	}

	uReturnCode_ = SCardListReaders(hContext_,NULL,pTempReaderList,&uBufferLen);
	if (uReturnCode_ != SCARD_S_SUCCESS )
	{		
		throw PcscException(uReturnCode_);
	}

	try 
	{		

		uReturnCode_ = parseReaderList(pTempReaderList,
									   ppReaderList,rNumReaders);

		delete [] pTempReaderList;
		return uReturnCode_;
	}
	catch (PcscException cException)
	{
		throw cException;
	}
	

	

	return uReturnCode_;
}

uint32 PcscReader::selectReader(PcscReaderName *pReader)
{
	if (pReader == NULL)
	{
		throw PcscException("Invalid Parameters");
	}

	pCurrentReader_ = pReader;

	return uReturnCode_;
}

uint32 PcscReader::getReaderCount(int8 * pReaderString)
{
	uint32 uReaderCount = 0;
	int8	*pCurrentReaderString;

	if (pReaderString == NULL)
	{
		throw PcscException("Invalid Parameters");
	}

	pCurrentReaderString = pReaderString;

	while (*pCurrentReaderString != 0)
	{
		uReaderCount++;
		pCurrentReaderString += strlen((const char *)pCurrentReaderString) + 1;
	}

	return uReaderCount;	
}

uint32 PcscReader::parseReaderList(int8 * pReaderString,
								   PcscReaderName ** ppReaderList,
								   uint32 & rNumReaders)
{
	uint32 uIndex;
	rNumReaders = getReaderCount(pReaderString);
	PcscReaderName *pTempReaderList;

	pTempReaderList = new PcscReaderName[rNumReaders];

	if (pTempReaderList == NULL)
	{
		throw PcscException("Insufficient Resources");
	}

	uIndex = 0;
	while (*pReaderString != 0)
	{
		pTempReaderList[uIndex].setNameBuffer(pReaderString);
		
		pReaderString += strlen((const char *)pReaderString) + 1;
		uIndex++;
	}

	*ppReaderList =  pTempReaderList;


	return uReturnCode_;
}

uint32 PcscReader::beginTransaction()
{
	uReturnCode_ = SCardBeginTransaction(hCardHandle_);

	if (uReturnCode_ != SCARD_S_SUCCESS )
	{		
		throw PcscException(uReturnCode_);
	}

	return uReturnCode_;
}
uint32 PcscReader::endTransaction()
{
	uReturnCode_ = SCardEndTransaction(hCardHandle_,SCARD_LEAVE_CARD);

	if (uReturnCode_ != SCARD_S_SUCCESS )
	{		
		throw PcscException(uReturnCode_);
	}

	return uReturnCode_;
}

