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

enum VALUE_BLOCK_OPERATION
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

	CString getErrorMessage(byte* pSw1Sw2);
	bool isMifareClassic(byte* pAtr);

	//Commands
	bool loadAuthKey(uint8 uKeyNumber, mifareAuthKey aKey );
	bool authenticate(byte uBlockNumber, MIFARE_KEY_TYPE eKeyType, byte uKeyNumber);
	void valueBlock(byte uBlockNumber, VALUE_BLOCK_OPERATION eTransType, int iAmount);
	void store(byte uBlockNumber, int iAmount);
	void decrement(byte uBlockNumber, int iAmount);
	void increment(byte uBlockNumber, int iAmount);
	int inquireAmount(byte uBlockNumber);
	void restoreAmount(byte uSourceBlock, byte uTargetBlock);
	byte* readBinary(byte uBlockNumber,byte uLen);
	byte* readBinary(byte uBlockNumber);
	bool updateBinary(byte uBlockNumber, byte* pData,byte uLen);
	bool updateBinary(byte uBlockNumber, byte* pData);
	
private:	
	AcsReader* pReader_;
	Apdu* pApdu_;
	Helper cHelper_;
};

#endif // !defined(AFX_MIFARECLASSIC_H__2C3652EF_85FF_4D02_946C_287806918D7A__INCLUDED_)
