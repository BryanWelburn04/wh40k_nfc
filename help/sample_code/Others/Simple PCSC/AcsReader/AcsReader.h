#pragma once
#include "AcsCommon.h"
#include "Apdu.h"
#include "Functor.h"

#define CFG_ATR_BUFFER_LEN	0x14

class AcsReader
{
public:
	virtual ~AcsReader();

	boolean isConnectionActive() const	{ return bConnectionActive_; }	
	uint32 getReturnCode() const		{ return uReturnCode_; }	
	uint32 getLastError() const			{ return uLastError_; }
	Apdu* getApduCommand() const		{ return pApduCommand_; }

	void setApduCommand(Apdu* apduCommand);	

	virtual long initialize() = 0;
	virtual void cleanup() = 0;

	void setSendCommandFunctor(void (*pFunc)(const uint8* data, const int len));
	void setRecvCommandFunctor(void (*pFunc)(const uint8* data, const int len));

	virtual long connect() = 0;
	virtual long connect(void *) = 0;
	virtual long disconnect() = 0;

	virtual long getAtr(uint8 *pAtr, uint32 &rAtrLen) = 0;

	virtual long sendControlCommand(Apdu* apdu) = 0;
	virtual long sendControlCommand(uint8 *pCommand,
								    uint32 uCommandLen,
									uint8 *pResponse,
									uint32 *pResponseLen) = 0;

	virtual long sendApduCommand() = 0;
	virtual long sendApduCommand(Apdu* apdu) = 0;
	virtual long sendApduCommand(uint8 * pCommand,
								 uint32 uCommandLen,
								 uint8 * pResponse,
								 uint32 &rResponseLen) = 0;

	//Polling Related Functions
	virtual uint32 setPollingList(int8 ** ppReaderList, uint32 uNumReaders) = 0;
	virtual uint32 startPolling(void *pReaderData, uint32 uNumReaders) = 0;
	virtual uint32 stopPolling() = 0;

	
protected:

	boolean bConnectionActive_;	
	uint32	uReturnCode_;	
    uint32	uLastError_;
	uint32	uOperationControlCode_;
	Apdu*	pApduCommand_;

	// delegate function pointers
	Functor cSendCommandFunctor_;
	Functor cReceiveCommandFunctor_;

	
	uint8	aAtr_[CFG_ATR_BUFFER_LEN];
	uint32  uAtrLen_;
	

};