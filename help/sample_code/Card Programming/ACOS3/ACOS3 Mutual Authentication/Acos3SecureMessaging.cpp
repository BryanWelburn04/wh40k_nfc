//===========================================================================================
// 
//  Author          : Brey Vaughn Lee
// 
//  File            : Acos3ReadWrite.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Contains Methods and Properties of ACOS3 Secure Messaging operations
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
#include "Des.h"

BYTE _aTempDataOut[255];
BYTE SW1, SW2;


int ENC_CBC(BYTE SeqNum[], BYTE buff[], int len, BYTE key[], int bDes)
{
	int i,j;
	int blocklen;
    BYTE iv[256];

	if(len % 8)
	{
		buff[len] = 0x80;
		len += 1;

		while (len % 8)
		{
			buff[len] = 0;
			len += 1;
		}
	}

	for(i = 0; i < 8; i++)
	{
		buff[i] ^= SeqNum[i];
	}
		
	//CipherKey = SessionKey;

	blocklen = len/8;

	//intial vector
	for(j = 0; j < 8; j++)
	{
		iv[j] = buff[j];
	}
	
	for(i = 0; i < blocklen; i++)
	{

		if (bDes == 1)
			DES(iv, key, 1);
		else
			DES3(iv, key, 1);

		for(j = 0; j < 8; j++)
		{
			buff[j + (8 * i)] = iv[j];
			iv[j] = buff[j + (8 * (i + 1))];
			iv[j] = iv[j] ^= buff[j + (8 * i)];		
		}
	}

	for (j = 0; j < (i * 8) + 1; j++)
	{
		_aTempDataOut[j] = buff[j];
	}

	return (len - 8);
}

int DEC_CBC(BYTE SeqNum[], BYTE buff[], int len, BYTE key[], int bDes)
{
	int i, j;
    int blocklen;
    BYTE XorBlk[8];
    BYTE lastblk[8];
	BYTE temp[256];

    if(len % 8)
		return -1;

    for (i = 0; i < 8; i++)
	{
		XorBlk[i] = SeqNum[i];
	}

	//CipherKey = key;

    blocklen = len / 8;
    
    for (i = 0; i < blocklen; i++)
	{
		for (j = 0; j < 8; j++)
		{
			lastblk[j] = buff[j + (8 * i)];
		}

		if(bDes == 1)
			DES2(lastblk, key, 0);
		else
			DES6(lastblk, key, 0);

		for(j = 0; j < 8; j++)
		{
			temp[j] = buff[j + (8 * i)];
		}

		for(j = 0; j < 8; j++)
		{
			buff[j + (8 * i)] = lastblk[j] ^= XorBlk[j];
		}

		for(j = 0; j < 8; j++)
		{
			XorBlk[j] = temp[j];
		}
        
	}
	
	for (j = 0; j< 8; j++)
	{
		XorBlk[j] = lastblk[j];
	}    

    return ((len) - 8);
}

uint8  Acos3::calculate8BytePadding (uint8 uNumber)
{	
	uint8 uRemainder;

	uRemainder = uNumber % 8;

	if (uRemainder == 0)
	{
		return 0;
	}	

	return 8 - uRemainder;
}

char* Acos3::readBinarySM(byte HiByte, 
						  byte LoByte, 
						  byte DataLen, 
						  byte* SequenceNumber, 
						  byte* SessionKey, 
						  byte* readBufferString, 
						  int bDes)
{
	char tempstr[262];	
	int index, tempval;
	int i, j, indx, lastblk;
	uint32 lengthToRead = 21;
    BYTE aMacBuffer[256];
    BYTE L, Pi;
	byte buffToSend[9];
	byte statusWord[2];

	byte readBuffer[256]; //0x0a
	
	// calculate new length to read due to SM
	lengthToRead = DataLen + 15 + calculate8BytePadding(DataLen); 

	// build TLV <89 04 CLA INS P1 P2><97 01 P3>
    aMacBuffer[0] = 0x89;		//TLV for command header
    aMacBuffer[1] = 0x04;		//Length of command header
    aMacBuffer[2] = 0x8C;		//CLA
    aMacBuffer[3] = 0xB0;		//INS
    aMacBuffer[4] = HiByte;		//P1
    aMacBuffer[5] = LoByte;		//P2
    aMacBuffer[6] = 0x97;		//TLV for Original P3
    aMacBuffer[7] = 0x01;		//Length of Original P3
    aMacBuffer[8] = DataLen;	//P3

	// increment SeqNum
    if ((SequenceNumber[7] + 1) == 256)
        SequenceNumber[6] += 1;
	else
		SequenceNumber[7] += 1; 

	//last block of buff will have the MAC
    lastblk = ENC_CBC(SequenceNumber, aMacBuffer, 9, SessionKey, bDes);
	
	for(indx = 0; indx < 255; indx++)
	{
		aMacBuffer[indx] = _aTempDataOut[indx];
	}

	//increment SeqNum
    if ((SequenceNumber[7] + 1) == 256)
		SequenceNumber[6] += 1;
    else
        SequenceNumber[7] += 1;

	buffToSend[0] = 0x97;
	buffToSend[1] = 0x01;
	buffToSend[2] = DataLen;
	buffToSend[3] = 0x8E;
	buffToSend[4] = 0x04;
	
	i = lastblk;
    for (indx = 5; indx< 9; indx++)
	{
		buffToSend[indx] = aMacBuffer[i];
        i += 1;
	}

	if (pReader_ == NULL)
        throw CardException("Connection is not yet established");

	Apdu* apdu  = new Apdu();
	byte * response = new byte[lengthToRead + 2];
	byte command[5] = { 0x8C, 0xB0, HiByte, LoByte, 0x09 };

    apdu->setCommand(command);
	apdu->setSendData(buffToSend);
	apdu->setSendLen(14);
	apdu->setReceiveLen(lengthToRead + 2);
	apdu->setReceiveData(response);
	apdu->setStatusWord(response + lengthToRead - 2);

	pReader_->sendApduCommand(apdu);

	if(apdu->getReceiveLen() == 2)	
	{
		apdu->setStatusWord(response);
	}
	else if (apdu->getReceiveLen() != lengthToRead)
	{
		delete apdu;
		delete [] response;
		throw CardException("Unexpected Reponse Length");
	}

	memcpy(statusWord,apdu->getStatusWord(),2);	

	if (statusWord[0] == 0x61)
	{
		delete apdu;			
		
		byte* readBuffer2;
		readBuffer2 = new byte[statusWord[1]];
		if(readBuffer2 == NULL)
		{
			delete [] response;			
			throw CardException("Insufficient Memory For Get Response");
		}

		getResponse(statusWord[1], readBuffer2);
		memcpy(readBuffer, readBuffer2, lengthToRead);
		delete [] readBuffer2;
	}
	else if(statusWord[0] != 0x90)
	{
		char * errorMessage = getErrorMessage(statusWord);			
		delete apdu;
		delete [] response;
		throw CardException(errorMessage ,statusWord);
	}
	else
	{
		memcpy(readBuffer, apdu->getReceiveData(), lengthToRead);

		delete apdu;
	}

	if (readBuffer[readBuffer[1] + 4] != 0x90 || readBuffer[readBuffer[1] + 5] != 0x00)
	{
		byte* sw;
		sw = new byte[2];

		sw[0] = readBuffer[2];
		sw[1] = readBuffer[3];

		char * errorMessage = getErrorMessage(sw);

		delete [] sw;
		delete [] response;
		throw CardException(errorMessage, sw);
	}
	

	delete [] response;

	//get ENC_DATA and decrypt
	L = readBuffer[1];  // Length of Encrypted Data + Padding Indicator
	Pi = readBuffer[2]; // Padding Indicator

	for(indx = 0; indx < L - 1; indx++)
	{
		aMacBuffer[indx] = readBuffer[indx + 3];
	}

	DEC_CBC(SequenceNumber, aMacBuffer, L - 1, SessionKey, bDes);

	sprintf( tempstr, "" );
	index = 0;	
	tempval = DataLen;
	while( aMacBuffer[index] != 0x80 )
	{	
		if( index < tempval )
		{
			sprintf( tempstr, "%s%c", tempstr, aMacBuffer[index] );
			readBufferString[index] = aMacBuffer[index] ;
		}

		index++;
	}

	SW1 = readBuffer[readBuffer[1] + 2 + 1 + 1];
	SW2 = readBuffer[readBuffer[1] + 2 + 1 + 1 + 1];

	aMacBuffer[0] = 0x89;
    aMacBuffer[1] = 0x04;
    aMacBuffer[2] = 0x8C;
    aMacBuffer[3] = 0xB0;
    aMacBuffer[4] = HiByte;
    aMacBuffer[5] = LoByte;
    aMacBuffer[6] = 0x87;
    aMacBuffer[7] = L;
    aMacBuffer[8] = Pi;

	i = 9;
    j = 3;

	for( indx = 0; indx < L; indx++)
	{
		aMacBuffer[i] = readBuffer[j];
        i += 1;
        j += 1;
	}     

    aMacBuffer[9 + L - 1] = 0x99;
    aMacBuffer[9 + L - 1 + 1] = 0x02;
    aMacBuffer[9 + L - 1 + 2] = SW1;
    aMacBuffer[9 + L - 1 + 3] = SW2;

    lastblk = ENC_CBC(SequenceNumber, aMacBuffer, L + 6 + 2 + 4, SessionKey, bDes);

	for (indx = lastblk; indx < lastblk + 4; indx++)
	{
		if (aMacBuffer[indx] != readBuffer[indx + 1])
		{
			return "MAC is Incorrect\n";
		}
        else
		{
			return NULL;
 		}            
	}
	return "Secure Messaging Failed\n";
}



char* Acos3::writeBinarySM(int casetype, 
						   byte HiByte, 
						   byte LoByte, 
						   byte DataLen, 
						   byte* dataToWrite, 
						   byte* SequenceNumber, 
						   byte* SessionKey, 
						   int bDes)
{	
	int tempLen;
	int lastblk;
	uint8 uPadding;
  
	BYTE ENCData[256];
	BYTE buff[256];

	byte buffToSend[256];
	int lengthToRead = 10;
	byte readBuffer[256];

	if (pReader_ == NULL)
        throw CardException("Connection is not yet established");

    if (dataToWrite == NULL || DataLen == 0)
		throw CardException("Data to write is not specified");	

	//write data to card
	Apdu* apdu  = new Apdu();
	if (apdu == NULL)
	{
		throw CardException("Isufficient Memory");	
	}
	
	byte* response = new byte[lengthToRead + 2];
	if (response == NULL)
	{
		throw CardException("Isufficient Memory");	
	}
	

	byte statusWord[2];

	if (casetype == 1)
	{
		if((SequenceNumber[7] + 1) == 256)
			SequenceNumber[6] += 1;
		else
			SequenceNumber[7] += 1;
		
		//enc
		ENC_CBC(SequenceNumber, dataToWrite, DataLen, SessionKey, bDes);

		uPadding = calculate8BytePadding(DataLen);
		
		tempLen = DataLen + uPadding;	

		memset(ENCData,0,sizeof(ENCData));
		memcpy(ENCData,dataToWrite,tempLen);

		//build tlv
		buff[0] = 0x89;			//TLV for Command
		buff[1] = 0x04;			//Length
		buff[2] = 0x80 | 0x0C;	//CLA*
		buff[3] = 0xD0;			//INS
		buff[4] = HiByte;		
		buff[5] = LoByte;

		buff[6] = 0x87;			//TLV for Encrypted Data
		buff[7] = tempLen + 1;	//Length 
		buff[8] = uPadding;		//Pi		

		memcpy(buff + 9 , ENCData, tempLen);
		

		lastblk = ENC_CBC(SequenceNumber, buff, 9 + tempLen, SessionKey, bDes);

		buffToSend[0] = 0x87;
		buffToSend[1] = tempLen + 1;
		buffToSend[2] = uPadding; 

		
		memcpy(buffToSend + 3, ENCData, tempLen);

		
		buffToSend[3 + tempLen] = 0x8E;
		buffToSend[3 + tempLen + 1] = 0x04;

		memcpy(buffToSend + tempLen + 5, buff + lastblk,4);

		byte command[5] = { 0x80 | 0x0C, 0xD0, HiByte, LoByte, 6 + 3 + tempLen};

		apdu->setCommand(command);
		apdu->setSendData(buffToSend);	
		apdu->setSendLen(tempLen + 14);
		apdu->setReceiveLen(lengthToRead + 2);
		apdu->setReceiveData(response);
		apdu->setStatusWord(response + lengthToRead);

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

		if (statusWord[0] == 0x61) //Get Reponse Required
		{			
			byte* readBuffer2;
			readBuffer2 = new byte[statusWord[1]];

			if (readBuffer2==NULL)
			{
				delete apdu;
				delete [] response;
				throw CardException("Isufficient Memory");
			}

			getResponse(statusWord[1], readBuffer2);
			
			memcpy(readBuffer, readBuffer2, lengthToRead);
			delete [] readBuffer2;
		}
		else if(statusWord[0] != 0x90)
		{
			char * errorMessage = getErrorMessage(statusWord);			
			
			delete apdu;
			delete [] response;
			throw CardException(errorMessage ,statusWord);
		}
		else 
		{			
			memcpy(readBuffer, apdu->getReceiveData(), lengthToRead);
		}

		if (readBuffer[2] != 0x90 || readBuffer[3] != 0x00)
		{
			byte sw[2];

			sw[0] = readBuffer[2];
			sw[1] = readBuffer[3];

			char * errorMessage = getErrorMessage(sw);
			delete apdu;
			delete [] response;
			throw CardException(errorMessage, sw);
		}		
	}
	

	if((SequenceNumber[7] + 1) == 256)
		SequenceNumber[6] += 1;
	else
		SequenceNumber[7] += 1;

	SW1 = readBuffer[2];
	SW2 = readBuffer[3];
		
	buff[0] = 0x89;
    buff[1] = 0x04;
    buff[2] = 0x8C;
    buff[3] = 0xD2;
    buff[4] = 0x00;
    buff[5] = 0x00;
    buff[6] = 0x99;
    buff[7] = 2;
    buff[8] = SW1;
    buff[9] = SW2;

	lastblk = ENC_CBC(SequenceNumber, buff, 10, SessionKey, bDes);

	delete apdu;
	delete [] response;

	return NULL;
	
}