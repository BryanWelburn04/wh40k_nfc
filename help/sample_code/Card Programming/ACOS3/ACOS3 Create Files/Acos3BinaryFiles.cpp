//===========================================================================================
// 
//  Author          : Brey Vaughn Lee
// 
//  File            : Acos3BinaryFiles.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Contains Methods and Properties of ACOS3 Binary Files related operations
// 
//  Date            : January 7, 2013
// 
//  Revision Traile : [Author] / [Date of modification] / [Details of Modifications done]
// 
// 
//=========================================================================================

#include "stdafx.h"
#include "Acos3.h"
#include "CardException.h"
#include "PcscException.h"

void Acos3::writeBinary(int casetype, byte highOffset,byte lowOffset,byte* dataToWrite, byte dataLen)
{
	if (pReader_ == NULL)
        throw CardException("Connection is not yet established");

    if (dataToWrite == NULL || dataLen == 0)
        throw CardException("Data to write is not specified");	

	//write data to card
	Apdu* apdu  = new Apdu();
	if (apdu == NULL)
	{
		throw CardException("Unable to Allocate New ADPU");
	}

	byte statusWord[2];
	byte response[2];
	byte command[5] = { 0x80, 0xD0, highOffset, lowOffset, dataLen};

    apdu->setCommand(command);
	apdu->setSendData(dataToWrite);	
	apdu->setSendLen(dataLen + 5);
	apdu->setReceiveLen(0x02);
	apdu->setReceiveData(response);
	apdu->setStatusWord(response);

	try
	{
		pReader_->sendApduCommand(apdu);
	}
	catch(PcscException propException)
	{		
		delete apdu;
		throw propException;
	}    

	memcpy(statusWord,apdu->getStatusWord(),2);
	if(statusWord[0] != 0x90)
	{
		char * errorMessage = getErrorMessage(statusWord);	
	
		delete apdu;
		throw CardException(errorMessage ,statusWord);
	}

	delete apdu;
	
}

byte* Acos3::readBinary(byte highOffset, byte lowOffset, byte lengthToRead)
{
	if (pReader_ == NULL)
        throw CardException("Connection is not yet established");

	Apdu* apdu  = new Apdu();
	byte command[5] = { 0x80, 0xB0, highOffset, lowOffset, lengthToRead};

	byte * response = new byte[lengthToRead + 2];
	
    apdu->setCommand(command);
	apdu->setSendData(NULL);
	apdu->setReceiveLen(lengthToRead+2);
	apdu->setReceiveData(response);
	apdu->setStatusWord(response+lengthToRead);


   pReader_->sendApduCommand(apdu);

	if(*(apdu->getStatusWord()) != 0x90)
		throw CardException(getErrorMessage(apdu->getStatusWord()),apdu->getStatusWord());

	return apdu->getResponse();
}

void  Acos3::readBinary(byte highOffset, byte lowOffset, byte *readBuffer, byte lengthToRead)
{
	if (pReader_ == NULL)
        throw CardException("Connection is not yet established");

	if (readBuffer == NULL || lengthToRead == 0)
		throw CardException("Invalid Buffer Size");

	byte command[5] = { 0x80, 0xB0, highOffset, lowOffset, lengthToRead};
	byte statusWord[2];

	Apdu* apdu  = new Apdu();
	if (apdu == NULL)
	{
		throw CardException("Unable to Allocate New ADPU");
	}

	byte * response = new byte[lengthToRead + 2];
	if (response == NULL)
	{
		delete apdu;
		throw CardException("Unable to Allocate New Read Buffer");
	}	

    apdu->setCommand(command);
	apdu->setSendLen(5);
	apdu->setSendData(NULL);
	apdu->setReceiveLen(lengthToRead+2);
	apdu->setReceiveData(response);
	apdu->setStatusWord(response+lengthToRead);

	try
	{
		pReader_->sendApduCommand(apdu);
	}
	catch(PcscException propException)
	{
		delete [] response;
		delete apdu;
		throw propException;
	}

	if(apdu->getReceiveLen()==2) //No Data Received
	{
		apdu->setStatusWord(response);
	}

	memcpy(statusWord,apdu->getStatusWord(),2);
	if(statusWord[0] != 0x90)
	{
		char * errorMessage = getErrorMessage(statusWord);		
	
		delete [] response;
		delete apdu;
		throw CardException(errorMessage ,statusWord);
	}	

	memcpy(readBuffer, apdu->getReceiveData(),lengthToRead);

	delete apdu;
	delete [] response;
}