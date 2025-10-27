#pragma once
#include "AcsReader.h"
#include "Helper.h"
#include "Winscard.h"
#include "PcscProvider.h"
#include "PcscException.h"

class PcscReader : public AcsReader
{
public:	

	boolean isConnectionActive() const	{ return bConnectionActive_; }

	//construtor /destruction
	PcscReader();
	PcscReader(string sReaderName);
	PcscReader(PcscReader & rhs, string sReaderName);
	virtual ~PcscReader();

	void initialize();	
	void cleanup();	

	void selectReader(void * pSelector);

	int32 connect();
	int32 connect(string sReaderName_);
	int32 connectDirect(CString readerName);
	int32 connect(uint32 uSlotNumber);	
	
	int32 disconnect();	

	int32 getAtr(uint8 *pAtr, uint32 &rAtrLen);
	void  getAtr(ByteArray &rAtr);

	int32 sendControlCommand(Apdu *pApdu);
	int32 sendControlCommand();
	int32 sendControlCommand(uint8 *pCommand,
							 uint32 uCommandLen,
							 uint8 *pResponse,
							 uint32 &rResponseLen,
							 DWORD uOperationControlCode);
	
	int32 sendApduCommand();
	int32 sendApduCommand(Apdu* pApdu);
	int32 sendApduCommand(uint8  *pCommand,
						  uint32  uCommandLen,
						  uint8  *pResponse,
						  uint32 &rResponseLen);	

	void listReaders(string *pReaders,
					 uint8  &rReaderCount);
	void listReaders(StringList &rReaderList);
	void listReaders(StringVector &rReaderList);

	//Polling Related
	void registerReaderList(string *pReaderList, uint8 uReaderCount);
	void getStatusChange(StatusChangeList & rStatusChangeList);
	int32 getStatus(string sReaderName);
	void cancel();

private:

	string sReaderName_;
	SCARDCONTEXT		hContext_;
	SCARDHANDLE			hCardHandle_;
	DWORD				uActiveProtocol_;	
	
	SCARD_READERSTATE	*pReaderStateList_;
	uint8				uReaderStateListCount_;

	PcscProvider        *PcscProvider_;
};