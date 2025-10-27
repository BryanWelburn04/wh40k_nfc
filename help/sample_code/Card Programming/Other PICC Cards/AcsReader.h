#pragma once
#include "AcsCommon.h"
#include "Apdu.h"
#include "Functor.h"

#define CFG_ATR_BUFFER_LEN	100

enum CARD_STATUS
{
	CARD_STATUS_UNKNOWN = 0,
	CARD_STATUS_FOUND,
	CARD_STATUS_NOT_FOUND,
	CARD_STATUS_ERROR
};

class StatusChangeData
{
public:
	uint8		uSlotNumber_;
	string		sReaderName;
	CARD_STATUS eStatus;
};

typedef list<StatusChangeData> StatusChangeList;

class AcsReader
{
public:
	AcsReader();
	virtual ~AcsReader();

	boolean isConnectionActive() const	{ return bConnectionActive_;}	
	uint32 getReturnCode() const		{ return uReturnCode_;		}	
	uint32 getLastError() const			{ return uLastError_;		}
	Apdu* getApduCommand() const		{ return pApduCommand_;		}

	void setApduCommand(Apdu* apduCommand);	
	
	void setSendCommandFunctor(void (*pFunc)(const byte* data, const int len));
	void setRecvCommandFunctor(void (*pFunc)(const byte* data, const int len));

	virtual void initialize() = 0;	
	virtual void cleanup()    = 0;

	virtual void listReaders(string *pReaders, uint8  &rReaderCount) = 0;
	virtual void listReaders(StringList &rReaderList) = 0;
	virtual void listReaders(StringVector &rReaderList) = 0;

	virtual void registerReaderList(string *pReaderList, uint8 uReaderCount) = 0;
	virtual void getStatusChange(StatusChangeList & rStatusChangeList) = 0;
	virtual void cancel() = 0;

	//virtual void selectReader(void * pSelector) = 0;
	virtual void selectReader(string sReaderName) = 0;

	virtual int32 connect() = 0;
	virtual int32 connect(string sReaderName) = 0; // for PCSC Readers
	virtual int32 connect(uint32 uSlotNumber) = 0; // for Proprietary Readers
	virtual int32 disconnect() = 0;

	virtual int32 getAtr(uint8 *pAtr, uint32 &rAtrLen) = 0;

	virtual int32 sendControlCommand(Apdu* apdu) = 0;
	virtual int32 sendControlCommand(uint8  *	pCommand,
									 uint32		uCommandLen,
									 uint8  *	pResponse,
									 uint32 &	rResponseLen) = 0;

	virtual int32 sendApduCommand() = 0;
	virtual int32 sendApduCommand(Apdu* apdu) = 0;
	virtual int32 sendApduCommand(uint8 * pCommand,
								  uint32  uCommandLen,
								  uint8 * pResponse,
								  uint32  &rResponseLen) = 0;
protected:
	boolean bConnectionActive_;	
	uint32	uReturnCode_;	
    uint32	uLastError_;
	uint32	uOperationControlCode_;
	Apdu*	pApduCommand_;

	uint8	aAtr_[CFG_ATR_BUFFER_LEN];
	DWORD  uAtrLen_;

	// delegate function pointers
	Functor cSendCommandFunctor_;
	Functor cReceiveCommandFunctor_;
};