#pragma once

#include "Apdu.h"
#include "PcscReader.h"
#include "PcscException.h"
#include "CardException.h"

#define CFG_ATR_SIZE 0x29

class CardSelector
{
public:	
	CString readCardType(uint8 *pAtr, uint32 rAtrLen); //Read the ATR String to get CardType
	CString getUid(uint8 *pUid, uint32 &rUidLength);

	PcscReader *pReader_;

	CardSelector();
	CardSelector(PcscReader *pPcscReader);
	virtual ~CardSelector();

};