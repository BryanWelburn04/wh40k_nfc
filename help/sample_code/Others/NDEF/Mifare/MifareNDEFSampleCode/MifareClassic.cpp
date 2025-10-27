// MifareClassic.cpp: implementation of the MifareClassic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MifareNDEFSampleCode.h"
#include "MifareClassic.h"
#include "CardException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MifareClassic::MifareClassic()
{
	
}

MifareClassic::MifareClassic(AcsReader* pReader)
{
	setReader(pReader);
}


MifareClassic::~MifareClassic()
{
	if (pApdu_ != NULL)
	{
		delete pApdu_;
	}
}

//////////////////////////////////////////////////////////////////////
// Implementation of accessor functions
//////////////////////////////////////////////////////////////////////

void MifareClassic::setReader(AcsReader* pReader)
{
	pApdu_ = new Apdu();
	pReader_ = pReader;
}

//////////////////////////////////////////////////////////////////////
// Implementation of other member functions
//////////////////////////////////////////////////////////////////////

bool MifareClassic::loadAuthKey(uint8 uKeyNumber, 
								mifareAuthKey aKey)
{	
	byte aCommand[5] = { 0xFF,					//Instruction Class
                         0x82,                  //Instruction code
						 0x00,					//Key Structure
                         uKeyNumber,            //Key Number
                         0x06 };                //Length of key
	byte aResponse[2];

    if (aKey == NULL)
	{
		throw CardException("Invalid key length");
	}

	if (aKey == NULL)
	{
		throw CardException("Insufficient Resource for new APDU");
	}
	
	pApdu_->setCommand(aCommand);
    pApdu_->setSendData(aKey);	
	pApdu_->setSendLen(5 + MIFARE_KEY_SIZE);
	pApdu_->setReceiveData(aResponse);
	pApdu_->setReceiveLen(0x02);
	pApdu_->setStatusWord(aResponse);

    pReader_->sendApduCommand(pApdu_);

    if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		return false;
	}
	else
	{
		return true;
	}	

	delete pApdu_;
}


bool MifareClassic::authenticate(byte uBlockNumber,
								 MIFARE_KEY_TYPE eKeyType,
								 byte uKeyNumber)
{
	byte aCommand[5] = { 0xFF,					//Instruction Class
                         0x86,                  //Instruction code
						 0x00,					//RFU
                         0x00,					//RFU
                         0x05 };                //Length of key

	byte aData[] = {0x01,              //Version
                    0x00,              //RFU
                    (byte)uBlockNumber, //Block Number
                    (byte)eKeyType,     //Key Type
                    uKeyNumber};        //Key Number

	byte aResponse[2];

	if(uKeyNumber < 0x00 && uKeyNumber > 0x20)
	{
		throw CardException("Key number is invalid");
	}
	
	pApdu_->setCommand(aCommand);
    pApdu_->setSendData(aData);	
	pApdu_->setSendLen(10);
	pApdu_->setReceiveData(aResponse);
	pApdu_->setReceiveLen(0x02);
	pApdu_->setStatusWord(aResponse);
    
    pReader_->sendApduCommand(pApdu_);

    if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		return false;
	}
	else
	{
		return true;
	}
	
	delete pApdu_;
}

void MifareClassic::valueBlock(byte uBlockNumber,
							   VALUE_BLOCK_OPERATION eTransType,
							   int iAmount)
{
	byte aCommand[5] = { 0xFF,	
						0xD7,	
						0x00,	
						uBlockNumber,
						0x05 };	
	byte aSendData[5];
	byte aResponse[2];

	aSendData[0] = (byte)eTransType;	
	aSendData[1] = (byte)(((long)iAmount >> 24 ) & 0xFF);
	aSendData[2] = (byte)(((long)iAmount >> 16 ) & 0xFF);
	aSendData[3] = (byte)(((long)iAmount >> 8 ) & 0xFF);
	aSendData[4] = (byte)((long)iAmount & 0xFF);

	pApdu_->setCommand(aCommand);
    pApdu_->setSendData(aSendData);	
	pApdu_->setSendLen(0x0A);
	pApdu_->setReceiveData(aResponse);
	pApdu_->setReceiveLen(0x02);
	pApdu_->setStatusWord(aResponse);
    	
	pReader_->setApduCommand(pApdu_);	
	pReader_->sendApduCommand();
	
    if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		throw CardException("Value Block Operation failed", pApdu_->getStatusWord());
	}	
	
	delete pApdu_;
}


void MifareClassic::store(byte uBlockNumber, int iAmount)
{
	valueBlock(uBlockNumber, STORE, iAmount);
}


void MifareClassic::decrement(byte uBlockNumber, int iAmount)
{
	valueBlock(uBlockNumber, DECREMENT, iAmount);
}


void MifareClassic::increment(byte uBlockNumber, int iAmount)
{
	valueBlock(uBlockNumber, INCREMENT, iAmount);
}


int MifareClassic::inquireAmount(byte uBlockNumber)
{
	byte* pReceiveAmount;
	byte* pApduResponse;
	byte aCommand[5] = {0xFF,
						0xB1,
						0x00,
						uBlockNumber,
						0x04 };	
	int iAmount;

	pReceiveAmount = new byte[6];

	if(pReceiveAmount == NULL)
	{
		return NULL;
	}

    pApdu_->setCommand(aCommand);
	pApdu_->setSendLen(0x05);
	pApdu_->setReceiveLen(0x06);
    pApdu_->setReceiveData(pReceiveAmount);

	pReader_->setApduCommand(pApdu_);	
	pReader_->sendApduCommand();

	pApdu_->setStatusWord(pReceiveAmount);
	
	pApduResponse = pReader_->getApduCommand()->getReceiveData();

	iAmount = pApduResponse[3];
	iAmount = iAmount + ( pApduResponse[2] * 256 );
	iAmount = iAmount + ( pApduResponse[1] * 256 * 256 );
	iAmount = iAmount + ( pApduResponse[0] * 256 * 256 * 256 );

	if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		throw CardException("Inquire Operation failed", pApdu_->getStatusWord());
	}

	return iAmount;	
}



void MifareClassic::restoreAmount(byte uSourceBlock, byte uTargetBlock)
{
	byte aResponse[2];
	byte aCommand[5] = {0xFF,
						0xD7,
						0x00,
						uSourceBlock,
						0x02 };	
	byte aSendData[2];
	
	aSendData[0] = 0x03;	
	aSendData[1] = uTargetBlock;
	
	pApdu_->setCommand(aCommand);
    pApdu_->setSendData(aSendData);	
	pApdu_->setSendLen(0x07);
	pApdu_->setReceiveLen(0x02);
    pApdu_->setReceiveData(aResponse);
	pApdu_->setStatusWord(aResponse);
	
	pReader_->setApduCommand(pApdu_);	
	pReader_->sendApduCommand();

    if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		throw CardException("Restore Value failed", pApdu_->getStatusWord());
	}

	delete pApdu_;
}


byte* MifareClassic::readBinary(byte uBlockNumber,byte uLen)
{	
	byte* pReceiveData;
	byte aCommand[5] = {0xFF,
						0xB0,
						0x00,
						uBlockNumber,
						uLen };	
	
	pReceiveData = new byte[uLen + 2];

	if(pReceiveData == NULL)
	{
		return NULL;
	}

	pApdu_->setCommand(aCommand);	
	pApdu_->setSendLen(0x05); 
	pApdu_->setReceiveLen(uLen + 2);
	pApdu_->setReceiveData(pReceiveData);

	pReader_->setApduCommand(pApdu_);	
	pReader_->sendApduCommand();

	pApdu_->setStatusWord(pReceiveData + uLen);

	if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		throw CardException("Read failed", pApdu_->getStatusWord());
	}

	return pReader_->getApduCommand()->getReceiveData();
	delete pApdu_;
}

byte* MifareClassic::readBinary(byte uBlockNumber)
{	
	byte* pReceiveData;
	byte aCommand[5] = {0xFF,
						0xB0,
						0x00,
						uBlockNumber,
						0x10 };	

	pReceiveData = new byte[12];

	if(pReceiveData == NULL)
	{
		return NULL;
	}

	pApdu_->setCommand(aCommand);	
	pApdu_->setSendLen(0x05);
	pApdu_->setReceiveLen(0x12);
	pApdu_->setReceiveData(pReceiveData);
	
	pReader_->setApduCommand(pApdu_);	
	pReader_->sendApduCommand();

	pApdu_->setStatusWord(pReceiveData + 16);

	if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		throw CardException("Read failed", pApdu_->getStatusWord());
	}

	return pReader_->getApduCommand()->getReceiveData();
	delete pApdu_;
}

bool MifareClassic::updateBinary(byte uBlockNumber, byte* pData,byte uLen)
{
	byte aCommand[5] = {0xFF,
						0xD6,
						0x00,
						uBlockNumber,
						uLen};
	byte aResponse[2];

    pApdu_->setCommand(aCommand);
    pApdu_->setSendData(pData);	
	pApdu_->setSendLen(uLen + 5);
	pApdu_->setReceiveLen(0x02);
	pApdu_->setReceiveData(aResponse);
    	
	pReader_->setApduCommand(pApdu_);	
	pReader_->sendApduCommand();

	pApdu_->setStatusWord(aResponse);
	
    if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		return false;
	}
	else
	{
		return true;
	}

	delete pApdu_;
}

bool MifareClassic::updateBinary(byte uBlockNumber, byte* pData)
{
	byte aCommand[5] = {0xFF,
						0xD6,
						0x00,
						uBlockNumber,
						0x10 };
	byte aResponse[2];

    pApdu_->setCommand(aCommand);
    pApdu_->setSendData(pData);	
	pApdu_->setSendLen(0x15);
	pApdu_->setReceiveLen(0x02);
	pApdu_->setReceiveData(aResponse);
    	
	pReader_->setApduCommand(pApdu_);	
	pReader_->sendApduCommand();

	pApdu_->setStatusWord(aResponse);

    if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		return false;
	}
	else
	{
		return true;
	}

	delete pApdu_;
}

