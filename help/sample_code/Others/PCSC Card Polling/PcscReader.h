#pragma once
#include "AcsReader.h"
#include "Helper.h"
#include "Winscard.h"
#include "PcscProvider.h"

typedef uint32 (*PcscPollingCallback) (uint32 uStatus, byte *pAtr, DWORD lAtrLen, void * pParam);

//Polling
enum POLL_STATUS
{
	POLL_STATUS_UNKNOWN = 0,
	POLL_STATUS_FOUND = 1,
	POLL_STATUS_NOT_FOUND = 2,
	POLL_STATUS_ERROR = 3,
	POLL_STATUS_NEW_READER_FOUND = 4,
	POLL_STATUS_READER_REMOVED = 5
};

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
	int32 connect(string sReaderName);
	int32 connect(uint32 uSlotNumber);	
	
	int32 disconnect();	

	int32 getAtr(uint8 *pAtr, uint32 &rAtrLen);
	void  getAtr(ByteArray &rAtr);

	int32 sendControlCommand(Apdu *pApdu);
	int32 sendControlCommand(uint8 *pCommand,
							 uint32 uCommandLen,
							 uint8 *pResponse,
							 uint32 &rResponseLen);
	
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
	uint32 setPollingCallback(PcscPollingCallback);
	uint32 setPollingList(int8 ** ppReaderList, uint32 uNumReaders);
	uint32 clearPollingList();
	uint32 getPollingList();
	uint32 startPolling(void *pReaderData, uint32 uNumReaders);
	uint32 stopPolling();
	void registerReaderList(string *pReaderList, uint8 uReaderCount);
	void getStatusChange(StatusChangeList & rStatusChangeList);
	int32 getStatus(string sReaderName);
	void cancel();

private:
	string				sReaderName_;

	SCARDCONTEXT		hContext_;
	SCARDHANDLE			hCardHandle_;
	DWORD				uActiveProtocol_;	
	
	SCARD_READERSTATE	*pReaderStateList_;
	uint8				uReaderStateListCount_;

	PcscProvider        *pPcscProvider_;

	PcscPollingCallback pPollingCallback_;	

	uint32 getStatusChange(void * pParam);
	static UINT MainPollingThread( LPVOID pParam );

};