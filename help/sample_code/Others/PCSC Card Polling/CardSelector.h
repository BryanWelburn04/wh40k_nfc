#pragma once

#include "Apdu.h"
#include "PcscReader.h"

#define CFG_ATR_SIZE 0x29

class CardSelector
{
public:	
	CString readCardType(uint8 *pAtr, uint32 uAtrLen); //Read the ATR String to get CardType
	CString getUid(uint8 *pUid, uint32 &uUidLength);

	PcscReader *pReader_;
	string sReaderName_;

	CardSelector();
	CardSelector(PcscReader *pPcscReader, string sReaderName);
	virtual ~CardSelector();

};