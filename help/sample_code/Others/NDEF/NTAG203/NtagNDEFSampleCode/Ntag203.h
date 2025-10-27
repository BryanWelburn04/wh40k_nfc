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

class Ntag203  
{
public:	
	// constructor / destructor
	Ntag203();	
	Ntag203(AcsReader *pReader);
	virtual ~Ntag203();

	// Setting the reader
	void setReader(AcsReader* pReader);	

	CString getErrorMessage(byte* pSw1Sw2);
	bool isNtag203(byte* pAtr);

	//Commands
	byte* readBinary(byte uBlockNumber,byte uLen);
	byte* readBinary(byte uBlockNumber);
	void updateBinary(byte uBlockNumber, byte* data,byte len);
	void updateBinary(byte uBlockNumber, byte* sendData);

	// public variable	
	Helper cHelper;

	

private:	
	AcsReader *pReader_;
	Apdu* pApdu;
};

#endif // !defined(AFX_MIFARECLASSIC_H__2C3652EF_85FF_4D02_946C_287806918D7A__INCLUDED_)
