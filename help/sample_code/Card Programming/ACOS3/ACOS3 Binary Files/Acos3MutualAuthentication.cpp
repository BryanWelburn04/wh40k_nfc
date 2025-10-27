//===========================================================================================
// 
//  Author          : Brey Vaughn Lee
// 
//  File            : Acos3MutualAuthentication.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Contains Methods and Properties of ACOS3 Mutual Authentication operations
// 
//  Date            : January 7, 2013
// 
//  Revision Traile : [Author] / [Date of modification] / [Details of Modifications done]
// 
// 
//=========================================================================================

#include "stdafx.h"
#include "Acos3.h"
#include "Apdu.h"
#include "CardException.h"


void Acos3::startSession(byte *buffer)
{
	if (pReader_ == NULL)
        throw CardException("Connection is not yet established");

	Apdu* apdu  = new Apdu();
	if (apdu == NULL)
	{
		throw CardException("Unable to Allocate New ADPU");
	}

	byte * response = new byte[0x0A];
	if (response == NULL)
	{
		delete apdu;
		throw CardException("Unable to Allocate New Read Buffer");
	}

	byte command[5] = { 0x80, 0x84, 0x00, 0x00, 0x08};
	byte stat[] = {0x90,0x00};
	byte statusWord[2];
	
	apdu->setCommand(command);
	apdu->setSendData(NULL);	
	apdu->setSendLen(5);
	apdu->setReceiveLen(0x0A);
	apdu->setReceiveData(response);
	apdu->setStatusWord(response+0x08);


   pReader_->sendApduCommand(apdu);

	memcpy(statusWord,apdu->getStatusWord(),2);
	if(statusWord[0] != 0x90)
	{
		char * errorMessage = getErrorMessage(statusWord);		
		
		delete apdu;
		delete [] response;

		throw CardException(errorMessage ,statusWord);
	}

	memcpy(buffer, apdu->getReceiveData(),0x08);

	delete apdu;
	delete [] response;
}

void Acos3::authenticate(byte * pEncryptedCardRandNumber, 
						 byte * pTermRandNumber,
						 byte * pEncryptedRandTermNumber)
{
	if (pReader_ == NULL)
	{
        throw CardException("Connection is not yet established");
	}

	if (pReader_ == NULL)
	{
        throw CardException("Invalid Parmeters");
	}

	Apdu* apdu  = new Apdu();
	if (apdu == NULL)
	{
		throw CardException("Insufficient");
	}

	byte response[30];
	byte command[5] = { 0x80, 0x82, 0x00, 0x00, 0x10};
	byte stat[] = {0x90,0x00};
	byte statusWord[2];

	apdu->setCommand(command);
	
	byte data[16];

	memcpy(data,pEncryptedCardRandNumber,8);
	memcpy(data+8,pTermRandNumber,8);
	
	apdu->setCommand(command);
	apdu->setSendData(data);	
	apdu->setSendLen(sizeof(data) + 5);
	apdu->setReceiveLen(sizeof(response));
	apdu->setReceiveData(response);
	apdu->setStatusWord(response+0x08);

	pReader_->sendApduCommand(apdu);
 
	if(apdu->getReceiveLen() == 2)
	{
		apdu->setStatusWord(response);
	}
 

	memcpy(statusWord,apdu->getStatusWord(),2);

	if(apdu->getReceiveLen() == 2)
	{
		apdu->setStatusWord(response);
	}

	if (statusWord[0]==0x61)
	{
		delete apdu;

		byte* cardResponse;
		cardResponse = new byte[8];
		if (cardResponse == NULL)
		{
			throw CardException("Insufficient");
		}

		getResponse(statusWord[1], cardResponse);
		memcpy(pEncryptedRandTermNumber, cardResponse, 8);

		delete [] cardResponse;
		return;
	}
	else if (statusWord[0]!=0x90)
	{
		char * errorMessage = getErrorMessage(statusWord);  
  
		delete apdu;
		//delete [] response;

		throw CardException(errorMessage ,statusWord);
	}

	memcpy(pEncryptedRandTermNumber, apdu->getReceiveData(),8);

	delete apdu;
	
}
