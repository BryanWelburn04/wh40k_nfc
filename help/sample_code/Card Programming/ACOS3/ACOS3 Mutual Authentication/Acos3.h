#pragma once

#include "AcsReader.h"
#include "Helper.h"
#include "HelperClass.h"
#include "Apdu.h"
#include "Des.h"

enum CODE_TYPE
{
    AC1 = 1,
    AC2 = 2,
    AC3 = 3,
    AC4 = 4,
    AC5 = 5,
    PIN = 6,
    IC = 7
};

enum INTERNAL_FILE
{
    MCUID_FILE = 0,
    MANUFACTURER = 1,
    PERSONALIZATION = 2,
    SECURITY_FILE = 3,
    USER_FILE_MGMT_FILE = 4,
    ACCOUNT_FILE = 5,
    ACCOUNT_SECURITY = 6,
    ATR = 7
};

enum CARD_INFO_TYPE
{
    CARD_SERIAL = 0,
    EEPROM = 5,
    VERSION_NUMBER = 6
};

typedef byte ACOS3_CODE[8];

class Acos3
{

private:
	uint8  calculate8BytePadding (uint8 uNumber);
	void   acos_AsmISOOutCommand(byte *pSessionKey, //Command without Data
							 byte *pSeqNum, //Incremented SeqNum
							 BOOLEAN b3DES,
							 byte * pBuffer,
							 uint32 & rLen,
							 byte * pPrevCmd);
public:
	
	void submitAppCodes(CODE_TYPE codeToSubmit, byte *code, byte * response);
	
	void setReader(AcsReader * pReader);

	// public variable	
	Helper helper;

	int8* getErrorMessage(byte* sw1sw2);
	Apdu* getApduCommand() const { return pApdu_; }

	void clearCard();
	void submitCode(CODE_TYPE eCodeToSubmit, ACOS3_CODE aCode);
	void changePin(byte* newPin);
	void selectFile(INTERNAL_FILE internalFile);
	void selectFile(byte* fileID);
	void writeRecord(byte recordNumber,byte offset,byte* dataToWrite, byte dataLen);
	void writeBinary(int32 casetype, byte highOffset,byte lowOffset,byte* dataToWrite, byte dataLen);
	void credit(byte* mac,byte* amount,byte* ttref);
	void revokeDebit(byte* mac);
	void startSession(byte *buffer);
	void getResponse(byte lenToReceived, byte* cardResponse);	
	void authenticate(byte * pEncryptedCardRandomNumber, 
					  byte * pTermRandomNumber, 
					  byte * pEncryptedTerminalRandomNumber);	
	byte* getCardInfo(CARD_INFO_TYPE cardInfoType);
	byte* readRecord(byte recordNumber,byte offset,byte lengthToRead);
	void  readRecord(byte recordNumber,byte offset, byte *readBuffer, byte lengthToRead);
	byte* readBinary(byte highOffset, byte lowOffset, byte lengthToRead);
	void  readBinary(byte highOffset, byte lowOffset, byte *readBuffer, byte lengthToRead);
	byte* debit(byte* mac,byte* amount,byte* ttref,byte p1);
	byte* inquireAccount(byte keyNumber,byte* randomNumber);

	void configurePersonalizationFile(OptionRegister* optionRegister, SecurityOptionRegister* securityRegister, byte NumberOfFiles);
	void createRecordFile(byte recordNumber, byte* fileID, byte numberOfRecords, byte recordLength,
                                     SecurityAttribute* writeSecurityAttribute, SecurityAttribute* readSecurityAttribute,
                                     bool readRequireSecureMessaging, bool writeRequireSecureMessaging);
	
	void Acos3DES (BYTE *Data, BYTE *Key, int bEncrypt);
	void Acos3DES3 (BYTE *Data, BYTE *Key, int bEncrypt);
	void Acos3DES2 (BYTE *Data, BYTE *Key, int bDecrypt);
	void Acos3DES6 (BYTE *Data, BYTE *Key, int bDecrypt);

	char* readBinarySM(byte HiByte, byte LoByte, byte DataLen, byte* SequenceNumber, byte* SessionKey, byte *readBuffer, int bDes);
	char* writeBinarySM(int casetype, byte HiByte, byte LoByte, byte DataLen, byte* dataToWrite, byte* SequenceNumber, byte* SessionKey, int bDes);
	
	// constructor / destructor
	Acos3();	
	Acos3(AcsReader * prop);

private:
	AcsReader * pReader_;
	Apdu* pApdu_;

};
