// Ntag203.cpp: implementation of the Ntag203 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ntag203.h"
#include "CardException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Ntag203::Ntag203()
{
	
}

Ntag203::Ntag203(AcsReader* pReader)
{
	setReader(pReader);
}


Ntag203::~Ntag203()
{

}

//////////////////////////////////////////////////////////////////////
// Implementation of accessor functions
//////////////////////////////////////////////////////////////////////

void Ntag203::setReader(AcsReader* pReader)
{
	pReader_ = pReader;
}

//////////////////////////////////////////////////////////////////////
// Implementation of other member functions
//////////////////////////////////////////////////////////////////////
byte* Ntag203::readBinary(byte uBlockNumber,byte len)
{		
	byte aCommand[5] = { 0xFF, 0xB0, 0x00, uBlockNumber, len };	
	byte *pReceiveData;

	pApdu = new Apdu();
	pReceiveData = new byte[len + 2];

	if(pReceiveData == NULL)
	{
		return NULL;
	}

	pApdu->setCommand(aCommand);	
	pApdu->setSendLen(0x05); 
	pApdu->setReceiveLen(len + 2);
	pApdu->setReceiveData(pReceiveData);
	pReader_->setApduCommand(pApdu);	
	pReader_->sendApduCommand();
	pApdu->setStatusWord(pReceiveData + len);

	if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		//delete apdu;
		throw CardException("Read failed", pApdu->getStatusWord());
	}

	return pReader_->getApduCommand()->getReceiveData();

	delete pReceiveData;
}

byte* Ntag203::readBinary(byte blockNumber)
{	
	byte aCommand[5] = { 0xFF, 0xB0, 0x00, blockNumber, 0x10 };	
	byte *pReceiveData;

	pApdu = new Apdu();
	pReceiveData = new byte[12];
	if(pReceiveData == NULL)
	{
		return NULL;
	}

	pApdu->setCommand(aCommand);	
	pApdu->setSendLen(0x05);
	pApdu->setReceiveLen(0x12);
	pApdu->setReceiveData(pReceiveData);
	
	pReader_->setApduCommand(pApdu);	
	pReader_->sendApduCommand();
	pApdu->setStatusWord(pReceiveData + 16);
 
	if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		throw CardException("Read failed", pApdu->getStatusWord());
	}

	return pReader_->getApduCommand()->getReceiveData();

	delete pReceiveData;
}

void Ntag203::updateBinary(byte blockNumber, byte* sendData, byte len)
{
	byte aCommand[5] = { 0xFF, 0xD6, 0x00, blockNumber, len};
	
	byte aResponse[2];

	pApdu = new Apdu();
    pApdu->setCommand(aCommand);
    pApdu->setSendData(sendData);	
	pApdu->setSendLen(len+5);
	pApdu->setReceiveLen(0x02);
	pApdu->setReceiveData(aResponse);
    	
	pReader_->setApduCommand(pApdu);	
	pReader_->sendApduCommand();
	pApdu->setStatusWord(aResponse);
	
    if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		throw CardException("Update failed", pApdu->getStatusWord());
	}

	delete pApdu;
}

void Ntag203::updateBinary(byte blockNumber, byte* sendData)
{
	byte aCommand[5] = { 0xFF, 0xD6, 0x00, blockNumber, 0x10 };
	
	byte aResponse[2];

	pApdu = new Apdu();
    pApdu->setCommand(aCommand);
    pApdu->setSendData(sendData);	
	pApdu->setSendLen(0x15);
	pApdu->setReceiveLen(0x02);
	pApdu->setReceiveData(aResponse);
    	
	pReader_->setApduCommand(pApdu);	
	pReader_->sendApduCommand();
	pApdu->setStatusWord(aResponse);


     if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		throw CardException("Update failed", pApdu->getStatusWord());
	}

	delete pApdu;
}
