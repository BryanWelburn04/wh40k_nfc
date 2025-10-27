//===========================================================================================
// 
//  Author          : Maria Czarina V. Burgos
// 
//  File            : PICCClass.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Contains Methods and API Wrappers for class PICCClass general operations
// 
//  Date            : March 20, 2014
// 
//  Revision Trail  :
//  Date              Author                  Description
//     
//=========================================================================================

#include "stdafx.h"
#include "PICCClass.h"
#include "CardException.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PICCClass::PICCClass()
{
	pApdu_ = NULL;
}

PICCClass::~PICCClass()
{
}

//////////////////////////////////////////////////////////////////////
// Implementation of other member functions
//////////////////////////////////////////////////////////////////////

void PICCClass::getData(byte uIso14443A, byte *pResponse, int &iLength)
{
	if (!isConnect())
		throw CardException("Connection is not yet established");

	Apdu* pApdu  = new Apdu();
	if (pApdu == NULL)
		throw CardException("Unable to Allocate New ADPU"); 

	byte uCommand[5] = { 0xFF, 0xCA, uIso14443A, 0x00, 0x00 };
	byte uStatusWord[2];

    pApdu->setCommand(uCommand);
	pApdu->setSendData(NULL);	
	pApdu->setSendLen(5);
	pApdu->setLengthExpected(0x04);
	pApdu->setReceiveLen(0xFF);
	pApdu->setReceiveData(pResponse);

	sendApduCommand(pApdu);

	iLength = getActualLength() - 2;

	pApdu->setStatusWord(pResponse + iLength);

	memcpy(uStatusWord, pApdu->getStatusWord(), 2);	
	if(uStatusWord[0] != 0x90)
	{
		memcpy(uStatusWord, pApdu->getStatusWord(), 2);	
		delete pApdu;
		throw CardException(getErrorMessage(uStatusWord), uStatusWord);
	}

	delete pApdu;	
}

void PICCClass::sendCommand(int iCaseType, byte uCla, byte uIns, byte uP1, byte uP2, byte uLc, byte uLe, byte* uData)
{
	if (!isConnect())
		throw CardException("Connection is not yet established");

	Apdu* pApdu  = new Apdu();
	if (pApdu == NULL)
		throw CardException("Unable to Allocate New ADPU"); 

    /////////////////////////////////////////////////
    // | CLA | INS | P1 | P2 | Lc | Data Field | Le |
    // |<----- Header ------>|<------- Body ------->|
    /////////////////////////////////////////////////

	byte uStatusWord[2];
	byte *pResponse = new byte[uLe + 2];
	int  iLength = 0;

    switch (iCaseType)
    {
        case 0: // Header
		case 2: // Header-Lc-Data
		case 3: // Header-Lc-Data-Le
		{
            byte uCommand[5] = { uCla, uIns, uP1, uP2, uLc };
			pApdu->setCommand(uCommand);
            break;
		}

        case 1: // Header Le
		{
			byte uCommand[5] = { uCla, uIns, uP1, uP2, uLe };
			pApdu->setCommand(uCommand);
            break;
		}
	}
	
	pApdu->setSendData(uData);
	pApdu->setSendLen(5 + uLc);
	pApdu->setReceiveLen(uLe + 2);
	pApdu->setReceiveData(pResponse);
	
	sendApduCommand(pApdu);

	iLength = getActualLength() - 2;

	pApdu->setStatusWord(pResponse + iLength);

	memcpy(uStatusWord, pApdu->getStatusWord(), 2);	
	if(uStatusWord[0] != 0x90)
	{
		memcpy(uStatusWord, pApdu->getStatusWord(), 2);	
		delete pApdu;
		delete pResponse;
		throw CardException(getErrorMessage(uStatusWord), uStatusWord);
	}

	delete pApdu;
    delete pResponse;
}

char* PICCClass::getErrorMessage(byte* pStatusWord)
{
	if (pStatusWord == NULL)
        throw CardException("Unknown Status Word (statusWord)");

	else if (pStatusWord[0] == 0x6A && pStatusWord[1] == 0x81)
        return "The function is not supported.";

	else if (pStatusWord[0] == 0x63 && pStatusWord[1] == 0x00)
        return "The operation failed.";

	else
	{
		static char cStatus[100];
		
		sprintf(cStatus,"Unknown Status Word (%02x%02x)", pStatusWord[0], pStatusWord[1]);

		return cStatus;
	}
}
