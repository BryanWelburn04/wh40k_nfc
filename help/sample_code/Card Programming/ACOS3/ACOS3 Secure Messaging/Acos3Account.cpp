//===========================================================================================
// 
//  Author          : Brey Vaughn Lee
// 
//  File            : Acos3Account.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Contains Methods and Properties of ACOS3 Account related operations
// 
//  Date            : January 7, 2013
// 
//  Revision Traile : [Author] / [Date of modification] / [Details of Modifications done]
// 
// 
//=========================================================================================

#include "stdafx.h"
#include "Acos3.h"
#include "PcscException.h"
#include "CardException.h"

void Acos3::credit(byte* mac,byte* amount,byte* ttref)
{
	if (pReader_ == NULL)
        throw CardException("Connection is not yet established");

    if (mac == NULL)
        throw CardException("MAC has invalid length. Valid length is 4 bytes");

    if (amount == NULL)
        throw CardException("Value has invalid length. Valid length is 3 bytes");

    if (ttref == NULL)
        throw CardException("Terminal transaction reference has invalid length. Valid length is 4 bytes");

	Apdu* apdu  = new Apdu();
	byte response[2];
	byte command[5] = { 0x80, 0xE2, 0x00, 0x00, 0x00};
	uint8 aStatusWord[2];

	byte data[11];
	int i;
	for (i = 0; i < 4; i++)
		data[i] = mac[i];
	for (i = 0; i < 3; i++)
		data[i + 4] = amount[i];
	for (i = 0; i < 4; i++)
		data[i + 7] = ttref[i];

	apdu->setCommand(command);
	apdu->setSendData(data);	
	apdu->setSendLen(sizeof(data) + 5);
	apdu->setReceiveLen(0x02);
	apdu->setReceiveData(response);
	apdu->setStatusWord(response);
	apdu->setParameter3(0x0B);

	pReader_->sendApduCommand(apdu);

	memcpy(aStatusWord,apdu->getStatusWord(),sizeof(aStatusWord));

	if(aStatusWord[0] != 0x90)
	{		
		delete apdu;
		
		throw CardException(getErrorMessage(aStatusWord),aStatusWord);
	}

	delete apdu;
}

void Acos3::revokeDebit(byte* mac)
{
	if (pReader_ == NULL)
        throw CardException("Connection is not yet established");

    if (mac == NULL)
        throw CardException("MAC has invalid length. Valid length is 4 bytes");

	Apdu* apdu  = new Apdu();
	byte response[2];
	byte command[5] = { 0x80, 0xE8, 0x00, 0x00, 0x04};
	byte aStatusWord[2];

	apdu->setCommand(command);
	apdu->setSendData(mac);	
	apdu->setSendLen(sizeof(mac) + 5);
	apdu->setReceiveLen(0x02);
	apdu->setReceiveData(response);
	apdu->setStatusWord(response);


	pReader_->sendApduCommand(apdu);

	memcpy(aStatusWord,apdu->getStatusWord(),sizeof(aStatusWord));

	if(aStatusWord[0] != 0x90)
	{		
		delete apdu;
		
		throw CardException(getErrorMessage(aStatusWord),aStatusWord);
	}

	delete apdu;
}

byte* Acos3::debit(byte* mac,byte* amount,byte* ttref,byte p1)
{
	byte* responseFromCard = NULL;

	if (pReader_ == NULL)
        throw CardException("Connection is not yet established");

    if (mac == NULL )
        throw CardException("MAC has invalid length. Valid length is 4 bytes");

    if (amount == NULL)
        throw CardException("Value has invalid length. Valid length is 3 bytes");

    if (ttref == NULL)
        throw CardException("Terminal transaction reference has invalid length. Valid length is 4 bytes");

	Apdu* apdu  = new Apdu();
	byte lengthExpected;
	byte *response;
	byte command[5] = { 0x80, 0xE6, p1, 0x00, 0x00};
	byte aStatusWord[2];

	byte data[11];
	int i;
	for (i = 0; i < 4; i++)
		data[i] = mac[i];
	for (i = 0; i < 3; i++)
		data[i + 4] = amount[i];
	for (i = 0; i < 4; i++)
		data[i + 7] = ttref[i];
	
	apdu->setCommand(command);
	apdu->setSendData(data);	
	apdu->setSendLen(sizeof(data) + 5);	
	
	apdu->setParameter3(0x0B);

	if(p1 == 0x00) // no certificate needed
	{
		response = new byte[2];
		if (response == NULL)
		{
			delete apdu;
			throw CardException("Insufficient memory to allocate response");
		}

		apdu->setReceiveLen(0x02);		
		apdu->setStatusWord(response);
	}
	else // certificate required
	{
		lengthExpected = 0x04;
		response = new byte[lengthExpected + 2];		
		if (response == NULL)
		{
			delete apdu;
			throw CardException("Insufficient memory to allocate response");
		}
		
		apdu->setReceiveLen(lengthExpected + 2);		
		apdu->setStatusWord(response + lengthExpected);
	}
	apdu->setReceiveData(response);

	try 
	{
		pReader_->sendApduCommand(apdu);

		int iResponseLen = apdu->getReceiveLen();		

		if (iResponseLen == 2) // reponse contains Sw1 and Sw2
		{ 
			apdu->setResponse(response);

			if (response[0] == 0x90) // Successful Operation, no certificate required
			{
				responseFromCard = apdu->getReceiveData();
				
				delete apdu;
				
				return responseFromCard;
			}
			else if (response[0] == 0x61) //response[0] = Sw1
			{
				uint8 uGetResponseLen = response[1]; //response[1] = Sw2
				getResponse(uGetResponseLen,response);

				delete apdu;

				return response;				
			}
			else
			{
				memcpy(aStatusWord,apdu->getResponse(),sizeof(aStatusWord));
				delete apdu;
				delete [] response;
				throw CardException(getErrorMessage(aStatusWord),aStatusWord);
			}			
		}
		else if (iResponseLen == 6)
		{
			//do nothing
		}
		else
		{
			delete apdu;
			delete [] response;
			throw CardException("Unexpected Response");
		}
	}
	catch (PcscException cException)
	{
		delete apdu;		
		delete [] response;

		throw cException;
	}

	memcpy(aStatusWord,apdu->getStatusWord(),sizeof(aStatusWord));
	
	if(aStatusWord[0] != 0x90)
	{		
		delete apdu;
		delete [] response;
		
		throw CardException(getErrorMessage(aStatusWord),aStatusWord);
	}
	else
	{
		if(apdu->getReceiveLen() == 0x02)
		{
			delete[] response;
			responseFromCard = NULL;
		}			
		else
		{
			// return to application; let application free this data
			responseFromCard = apdu->getReceiveData();
		}
	}

	delete apdu;
	return responseFromCard;
}

byte* Acos3::inquireAccount(byte keyNumber,byte* randomNumber)
{
	byte *pOutput;	

	InquireAccountResponse* inquire = new InquireAccountResponse();

	if (inquire == NULL)
		throw CardException("Insufficient Memory to allocate inquire response");	

	if (pReader_ == NULL)
		throw CardException("Connection is not yet established");        

    if (randomNumber == NULL)
		throw CardException("Random data has invalid length. Random data should be 4 bytes long");

	
	Apdu* apdu  = new Apdu();
	if (apdu == NULL)
	{
		delete inquire;
		throw CardException("Insufficient Memory to allocate apdu");
	}	

	byte lengthExpected = 0x19;
	byte* response = new byte[lengthExpected + 2];
	if (response == NULL)
	{
		delete apdu;
		delete inquire;		
		throw CardException("Insufficient Memory to allocate response");
	} 

	byte command[5] = { 0x80, 0xE4, keyNumber, 00, sizeof(randomNumber)};
	byte aStatusWord[2];
	
	apdu->setCommand(command);
	apdu->setSendData(randomNumber); 
	apdu->setSendLen(sizeof(randomNumber) + 5);
	apdu->setReceiveLen(lengthExpected + 2);
	apdu->setReceiveData(response);
	apdu->setStatusWord(response + lengthExpected);

	try
	{
		pReader_->sendApduCommand(apdu);
	}
	catch (PcscException cException)
	{
		delete apdu;
		delete inquire;
		delete [] response;

		throw CardException("Card Reader Error");
	}

	uint32 uResponseLen = apdu->getReceiveLen();

	if (uResponseLen == 2 ) // Need to call GetResponse
	{
		apdu->setStatusWord(response);
	}
	else if (uResponseLen == 0x19 + 2)
	{
		//do nothing
	}
	else
	{
		throw CardException("Unexpected Response Length");
	}
	
	memcpy(aStatusWord,apdu->getStatusWord(),sizeof(aStatusWord));

	if(aStatusWord[0] == 0x61)
	{
		uint8 uGetResponseLen = aStatusWord[1];
				
		try 
		{
			getResponse(uGetResponseLen, response);			
		}
		catch (CardException cCardException)
		{			
			delete apdu;
			delete inquire;
			delete [] response;
			throw CardException("GetResponse Fail");
		}
		catch (PcscException cPropException)
		{		
			delete apdu;
			delete inquire;
			delete [] response;
			throw PcscException("GetResponse Fail");
		}	
		
	}
	else if(aStatusWord[0] != 0x90)
	{
		delete apdu;
		delete inquire;
		delete [] response;
		throw CardException(getErrorMessage(aStatusWord),aStatusWord);
	}	

	pOutput = inquire->inquireAccountResponse(response);

	delete apdu;
	delete inquire;	
	
	return pOutput;
}