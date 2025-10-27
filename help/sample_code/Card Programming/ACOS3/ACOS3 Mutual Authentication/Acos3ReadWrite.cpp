//===========================================================================================
// 
//  Author          : Brey Vaughn Lee
// 
//  File            : Acos3ReadWrite.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Contains Methods and Properties of ACOS3 Record related operations
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

void Acos3::writeRecord(byte recordNumber, byte offset, byte* dataToWrite, byte dataLength)
{
	if (pReader_ == NULL)
        throw CardException("Connection is not yet established");

    if (dataToWrite == NULL || dataLength == 0)
        throw CardException("Data to write is not specified");	

	byte command[5] = { 0x80, 0xD2, recordNumber, offset, dataLength };
	byte resp[2];
	byte statusWord[2];

	Apdu* apdu  = new Apdu();
	if (apdu == NULL)
	{
		throw CardException("Unable to Allocate New ADPU");
	}	

    apdu->setCommand(command);
	apdu->setSendData(dataToWrite);	
	apdu->setSendLen(dataLength + 5);
	apdu->setReceiveData(resp);	
	apdu->setReceiveLen(0x02);
	apdu->setStatusWord(resp);

	pReader_->sendApduCommand(apdu);

	memcpy(statusWord,apdu->getStatusWord(),2);
	if(statusWord[0] != 0x90)
	{
		char * errorMessage = getErrorMessage(statusWord);		
		
		delete apdu;
		throw CardException(errorMessage ,statusWord);
	}

	delete apdu;
}

void Acos3::readRecord(byte recordNumber,
					   byte offset, 
					   byte *readBuffer, 
					   byte lengthToRead)
{
	if (pReader_ == NULL)
        throw CardException("Connection is not yet established");

	byte command[5] = { 0x80, 0xB2, recordNumber, offset, lengthToRead};
	byte statusWord[2];

	Apdu* apdu  = new Apdu();
	if (apdu == NULL)
	{
		throw CardException("Unable to Allocate New ADPU");
	}

	byte * pResponse = new byte[lengthToRead + 2];
	if (pResponse == NULL)
	{
		delete apdu;
		throw CardException("Unable to Allocate New Read Buffer");
	}	

    apdu->setCommand(command);
	apdu->setSendData(NULL);
	apdu->setSendLen(5);
	apdu->setReceiveLen(lengthToRead + 2);
	apdu->setReceiveData(pResponse);
	apdu->setStatusWord(pResponse + lengthToRead);

	pReader_->sendApduCommand(apdu);


	memcpy(statusWord,apdu->getStatusWord(),2);
	if(statusWord[0] != 0x90)
	{
		char * errorMessage = getErrorMessage(statusWord);		
		
		delete apdu;
		delete [] pResponse;

		throw CardException(errorMessage ,statusWord);
	}

	memcpy(readBuffer, apdu->getReceiveData(),lengthToRead);

	delete apdu;
	delete [] pResponse;
}