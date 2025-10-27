// MifareClassic.h: interface for the MifareClassic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIFARECLASSIC_H__2C3652EF_85FF_4D02_946C_287806918D7A__INCLUDED_)
#define AFX_MIFARECLASSIC_H__2C3652EF_85FF_4D02_946C_287806918D7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AcsReader.h"
#include "Helper.h"	

#define MIFARE_KEY_SIZE	6

typedef uint8 mifareAuthKey[MIFARE_KEY_SIZE];

enum VALUEBLOCKOPERATION
{
	STORE = 0,
    INCREMENT = 1,
    DECREMENT = 2
};

enum MIFARE_KEY_TYPE
{
	KEY_TYPE_A = 0x60,
	KEY_TYPE_B = 0x61
};

class MifareClassic  
{
public:	
	// constructor / destructor
	MifareClassic();	
	MifareClassic(AcsReader *pReader);
	virtual ~MifareClassic();

	// Setting the reader
	void setReader(AcsReader* pReader);	

	CString getErrorMessage(byte* sw1sw2);
	bool isMifareClassic(byte* atr);

	//Commands
	void loadAuthKey(uint8 uKeyNumber, mifareAuthKey aKey );
	void authenticate(byte blockNumber, MIFARE_KEY_TYPE keytype, byte KeyNumber);
	void valueBlock(byte blockNumber, VALUEBLOCKOPERATION transType, int amount);
	void store(byte blockNumber, int amount);
	void decrement(byte blockNumber, int amount);
	void increment(byte blockNumber, int amount);
	int inquireAmount(byte blockNumber);
	void restoreAmount(byte sourceBlock, byte targetBlock);
	byte* readBinary(byte blockNumber,byte len);
	byte* readBinary(byte blockNumber);
	void updateBinary(byte blockNumber, byte* data,byte len);
	void updateBinary(byte blockNumber, byte* sendData);

	// public variable	
	Helper helper;

	

private:	
	AcsReader *pReader_;
};

#endif // !defined(AFX_MIFARECLASSIC_H__2C3652EF_85FF_4D02_946C_287806918D7A__INCLUDED_)
