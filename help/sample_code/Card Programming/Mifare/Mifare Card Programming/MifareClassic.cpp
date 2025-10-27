// MifareClassic.cpp: implementation of the MifareClassic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mifare Card Programming.h"
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

}

//////////////////////////////////////////////////////////////////////
// Implementation of accessor functions
//////////////////////////////////////////////////////////////////////

void MifareClassic::setReader(AcsReader* pReader)
{
	pReader_ = pReader;
}

//////////////////////////////////////////////////////////////////////
// Implementation of other member functions
//////////////////////////////////////////////////////////////////////

void MifareClassic::loadAuthKey(uint8 uKeyNumber, 
								mifareAuthKey pKey)
{	
    if (pKey == NULL)
	{
		throw CardException("Invalid key length");
	}

	Apdu *pApdu = new Apdu();
	if (pApdu == NULL)
	{
		throw CardException("Insufficient Resource for new APDU");
	}

	byte aCommand[5] = { 0xFF,					//Instruction Class
                         0x82,                  //Instruction code
						 0x00,					//Key Structure
                         uKeyNumber,            //Key Number
                         0x06 };                //Length of key

	byte aResponse[2];
	
	pApdu->setCommand(aCommand);
    pApdu->setSendData(pKey);	
	pApdu->setSendLen(5+MIFARE_KEY_SIZE);
	pApdu->setReceiveData(aResponse);
	pApdu->setReceiveLen(0x02);
	pApdu->setStatusWord(aResponse);

    pReader_->sendApduCommand(pApdu);

    if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		throw CardException("Load Key failed", pApdu->getStatusWord());
	}

	delete pApdu;
}


void MifareClassic::authenticate(byte blockNumber, 
								MIFARE_KEY_TYPE keyType, 
								byte KeyNumber)
{	

	if(KeyNumber < 0x00 && KeyNumber > 0x20)
	{
		throw CardException("Key number is invalid");
	}
    

	Apdu* pApdu = new Apdu();

	byte aCommand[5] = { 0xFF,					//Instruction Class
                         0x86,                  //Instruction code
						 0x00,					//RFU
                         0x00,					//RFU
                         0x05 };                //Length of key

	byte aData[] = {0x01,              //Version
                    0x00,              //RFU
                    (byte)blockNumber, //Block Number
                    (byte)keyType,     //Key Type
                    KeyNumber};        //Key Number

	byte aResponse[2];
	
	pApdu->setCommand(aCommand);
    pApdu->setSendData(aData);	
	pApdu->setSendLen(10);
	pApdu->setReceiveData(aResponse);
	pApdu->setReceiveLen(0x02);
	pApdu->setStatusWord(aResponse);
    
    pReader_->sendApduCommand(pApdu);

    if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		throw CardException("Authenticate failed", pApdu->getStatusWord());
	}

	delete pApdu;
}

void MifareClassic::valueBlock(byte blockNumber, VALUEBLOCKOPERATION transType, int amount)
{
	Apdu* apdu  = new Apdu();

	byte command[5] = { 0xFF, 0xD7, 0x00, blockNumber, 0x05 };	
	
	byte sendData[5];	
	sendData[0] = (byte)transType;	
	sendData[1] = (byte)(( (long)amount >> 24 ) & 0xFF);
	sendData[2] = (byte)(( (long)amount >> 16 ) & 0xFF);
	sendData[3] = (byte)(( (long)amount >> 8 ) & 0xFF);
	sendData[4] = (byte)(( long)amount & 0xFF);

	byte aResponse[2];

	apdu->setCommand(command);
    apdu->setSendData(sendData);	
	apdu->setSendLen(0x0A);
	apdu->setReceiveData(aResponse);
	apdu->setReceiveLen(0x02);
	apdu->setStatusWord(aResponse);
    	
	pReader_->setApduCommand(apdu);	
	pReader_->sendApduCommand();
	
    if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		throw CardException("Value Block Operation failed", apdu->getStatusWord());
	}
		
}


void MifareClassic::store(byte blockNumber, int amount)
{
	valueBlock(blockNumber, STORE, amount);
}


void MifareClassic::decrement(byte blockNumber, int amount)
{
	valueBlock(blockNumber, DECREMENT, amount);
}


void MifareClassic::increment(byte blockNumber, int amount)
{
	valueBlock(blockNumber, INCREMENT, amount);
}


int MifareClassic::inquireAmount(byte blockNumber)
{
	Apdu* apdu  = new Apdu();
	byte *receiveAmount;

	byte command[5] = { 0xFF, 0xB1, 0x00, blockNumber, 0x04 };	

	receiveAmount = new byte[6];
		if(receiveAmount == NULL)
	{
		return NULL;
	}

    apdu->setCommand(command);
	apdu->setSendLen(0x05);
	apdu->setReceiveLen(0x06);
    apdu->setReceiveData(receiveAmount);	
	pReader_->setApduCommand(apdu);	
	pReader_->sendApduCommand();
	apdu->setStatusWord(receiveAmount);
	
	byte* apduResponse = pReader_->getApduCommand()->getReceiveData();

	int amount;
	amount = apduResponse[3];
	amount = amount + ( apduResponse[2] * 256 );
	amount = amount + ( apduResponse[1] * 256 * 256 );
	amount = amount + ( apduResponse[0] * 256 * 256 * 256 );

	return amount;

	if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		throw CardException("Inquire Operation failed", apdu->getStatusWord());
	}
		
}



void MifareClassic::restoreAmount(byte sourceBlock, byte targetBlock)
{
	Apdu* apdu  = new Apdu();
	byte aResponse[2];

	byte command[5] = { 0xFF, 0xD7, 0x00, sourceBlock, 0x02 };	

	byte sendData[2];	
	sendData[0] = 0x03;	
	sendData[1] = targetBlock;
	
	apdu->setCommand(command);
    apdu->setSendData(sendData);	
	apdu->setSendLen(0x07);
	apdu->setReceiveLen(0x02);
    apdu->setReceiveData(aResponse);
	apdu->setStatusWord(aResponse);
	
	pReader_->setApduCommand(apdu);	
	pReader_->sendApduCommand();
	
    if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		throw CardException("Restore Value failed", apdu->getStatusWord());
	}
}


byte* MifareClassic::readBinary(byte blockNumber,byte len)
{	
	Apdu* apdu = new Apdu();	
	byte command[5] = { 0xFF, 0xB0, 0x00, blockNumber, len };	
	byte *receiveData;

	receiveData = new byte[len + 2];

	if(receiveData == NULL)
	{
		return NULL;
	}

	apdu->setCommand(command);	
	apdu->setSendLen(0x05); 
	apdu->setReceiveLen(len + 2);
	apdu->setReceiveData(receiveData);
	pReader_->setApduCommand(apdu);	
	pReader_->sendApduCommand();
	apdu->setStatusWord(receiveData + len);

	if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		//delete apdu;
		throw CardException("Read failed", apdu->getStatusWord());
	}

	return pReader_->getApduCommand()->getReceiveData();
}

byte* MifareClassic::readBinary(byte blockNumber)
{	
	Apdu* apdu = new Apdu();
	byte command[5] = { 0xFF, 0xB0, 0x00, blockNumber, 0x10 };	
	byte *receiveData;

	receiveData = new byte[12];
	if(receiveData == NULL)
	{
		return NULL;
	}

	apdu->setCommand(command);	
	apdu->setSendLen(0x05);
	apdu->setReceiveLen(0x12);
	apdu->setReceiveData(receiveData);
	
	pReader_->setApduCommand(apdu);	
	pReader_->sendApduCommand();
	apdu->setStatusWord(receiveData + 16);
 
	if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		throw CardException("Read failed", apdu->getStatusWord());
	}

	return pReader_->getApduCommand()->getReceiveData();
}

void MifareClassic::updateBinary(byte blockNumber, byte* sendData, byte len)
{
    Apdu* apdu  = new Apdu();
	byte command[5] = { 0xFF, 0xD6, 0x00, blockNumber, len};
	
	byte aResponse[2];

    apdu->setCommand(command);
    apdu->setSendData(sendData);	
	apdu->setSendLen(len+5);
	apdu->setReceiveLen(0x02);
	apdu->setReceiveData(aResponse);
    	
	pReader_->setApduCommand(apdu);	
	pReader_->sendApduCommand();
	apdu->setStatusWord(aResponse);
	
    if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		throw CardException("Update failed", apdu->getStatusWord());
	}
	delete apdu;
}

void MifareClassic::updateBinary(byte blockNumber, byte* sendData)
{
    Apdu* apdu  = new Apdu();
	byte command[5] = { 0xFF, 0xD6, 0x00, blockNumber, 0x10 };
	
	byte aResponse[2];

    apdu->setCommand(command);
    apdu->setSendData(sendData);	
	apdu->setSendLen(0x15);
	apdu->setReceiveLen(0x02);
	apdu->setReceiveData(aResponse);
    	
	pReader_->setApduCommand(apdu);	
	pReader_->sendApduCommand();
	apdu->setStatusWord(aResponse);


     if (pReader_->getApduCommand()->getStatusWord()[0] != 0x90)
	{
		throw CardException("Update failed", apdu->getStatusWord());
	}
	delete apdu;
}
