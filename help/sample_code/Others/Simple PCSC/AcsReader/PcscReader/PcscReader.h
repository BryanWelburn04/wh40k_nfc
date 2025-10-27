#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Winscard.h"
#include "ACSCommon.h"
#include "Functor.h"
#include "Apdu.h"
#include "AcsReader.h"
#include "PcscReaderName.h"


#define CFG_ATR_BUFFER_LEN	0x14

#define SCARD_E_UNEXPECTED				-2146435041 //  An unexpected card error has occurred.
#define SCARD_E_ICC_INSTALLATION		-2146435040 //  No Primary Provider can be found for the smart card.
#define SCARD_E_ICC_CREATEORDER			-2146435039 //  The requested order of object creation is not supported.
#define SCARD_E_UNSUPPORTED_FEATURE		-2146435038 //  This smart card does not support the requested feature.
#define SCARD_E_DIR_NOT_FOUND			-2146435037 //  The identified directory does not exist in the smart card.
#define SCARD_E_FILE_NOT_FOUND			-2146435036 //  The identified file does not exist in the smart card.
#define SCARD_E_NO_DIR                  -2146435035 //  The supplied path does not represent a smart card directory.
#define SCARD_E_NO_FILE                 -2146435034 //  The supplied path does not represent a smart card file.
#define SCARD_E_NO_ACCESS               -2146435033 //  Access is denied to this file.
#define SCARD_E_WRITE_TOO_MANY          -2146435032 //  The smartcard does not have enough memory to store the information.
#define SCARD_E_BAD_SEEK                -2146435031 //  There was an error trying to set the smart card file object pointer.
#define SCARD_E_INVALID_CHV             -2146435030 //  The supplied PIN is incorrect.
#define SCARD_E_UNKNOWN_RES_MNG         -2146435029 //  An unrecognized error code was returned from a layered component.
#define SCARD_E_NO_SUCH_CERTIFICATE     -2146435028 //  The requested certificate does not exist.
#define SCARD_E_CERTIFICATE_UNAVAILABLE -2146435027 //  The requested certificate could not be obtained.
#define SCARD_E_NO_READERS_AVAILABLE    -2146435026 //  Cannot find a smart card reader.
#define SCARD_E_COMM_DATA_LOST          -2146435025 //  A communications error with the smart card has been detected.  Retry the operation.
#define SCARD_E_NO_KEY_CONTAINER        -2146435024 //  The requested key container does not exist on the smart card.
#define SCARD_E_SERVER_TOO_BUSY         -2146435023 //  The Smart card resource manager is too busy to complete this operation.

// These are warning codes.
#define SCARD_W_SECURITY_VIOLATION		-2146434966 //  Access was denied because of a security violation.
#define SCARD_W_WRONG_CHV               -2146434965 //  The card cannot be accessed because the wrong PIN was presented.
#define SCARD_W_CHV_BLOCKED             -2146434964 //  The card cannot be accessed because the maximum number of PIN entry attempts has been reached.
#define SCARD_W_EOF                     -2146434963 //  The end of the smart card file has been reached.
#define SCARD_W_CANCELLED_BY_USER       -2146434962 //  The action was cancelled by the user.
#define SCARD_W_CARD_NOT_AUTHENTICATED  -2146434961 //  No PIN was presented to the smart card.
#define SCARD_W_CACHE_ITEM_NOT_FOUND    -2146434960 //  The requested item could not be found in the cache.
#define SCARD_W_CACHE_ITEM_STALE        -2146434959 //  The requested cache item is too old and was deleted from the cache.
#define SCARD_W_CACHE_ITEM_TOO_BIG      -2146434958 //  The new cache item exceeds the maximum per-item size defined for the cache.

typedef uint32 (*PcscPollingCallback) (uint32 uStatus, void * pParam);

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
private:
	uint32	uSlotNumber_;
	uint32	uActiveProtocol_;
	PcscPollingCallback pPollingCallback_;	

public:
	PcscReader();
	PcscReader(int8 * pPortName);

	//
	long initialize();
	void cleanup();

	//Generic Card Reader Operations: Implementation of base class ProprietaryReader
	long connect();
	long connect(void *); // void is pointer to int8
	long disconnect();

	long getAtr(uint8 *pAtr, uint32 &rAtrLen);

	long sendControlCommand();
	long sendControlCommand(Apdu *apdu);
	long sendControlCommand(uint8  *pCommand,
							uint32 uCommandLen,
							uint8  *pResponse,
							uint32 *pResponseLen);
	
	long sendApduCommand();
	long sendApduCommand(Apdu *apdu);
	long sendApduCommand(uint8  *pCommand,
						 uint32 uCommandLen,
						 uint8  *pResponse,
						 uint32 &rResponseLen);	
	//PCSC Specific Methods
	uint32 listReaders(PcscReaderName ** ppReaderList, uint32 & rNumReaders);
	uint32 selectReader(PcscReaderName * pReader);
	uint32 beginTransaction();
	uint32 endTransaction();

	PcscReaderName * getReaderName() const			 
	{ return pCurrentReader_; }
	void setReaderName(PcscReaderName * pReaderName) 
	{ pCurrentReader_ = pReaderName; }	
	uint32 getActiveProtocol() const                 
	{ return uActiveProtocol_; }
	uint32 getCurrenState() const 
	{ return uCurrentState_; }	
	int32 getOperationControlCode() const            
	{ return uOperationControlCode_; }	

	//Polling
	uint32 setPollingCallback(PcscPollingCallback);
	uint32 setPollingList(int8 ** ppReaderList, uint32 uNumReaders);
	uint32 clearPollingList();
	uint32 getPollingList();
	uint32 startPolling(void *pReaderData, uint32 uNumReaders);
	uint32 stopPolling();

private:
	uint32 getStatusChange(void * pParam);

private:
	SCARDCONTEXT hContext_;
	SCARDHANDLE  hCardHandle_;
	HANDLE hReader_;
    int8 aReaderPort_[255];		//TODO: Set as configurable
	PcscReaderName *pCurrentReader_;
	int32 uOperationControlCode_;
	uint32 uCurrentState_;
	uint32 uCurrentProtocol_;

	SCARD_IO_REQUEST tIoRequest_;

private:
	uint32 getReaderCount(int8 * pReaderString);
	uint32 parseReaderList(int8 * pReaderString,
						   PcscReaderName ** ppReaderList,
						   uint32 & rNumReaders);
	static UINT MainPollingThread( LPVOID pParam );
	
};