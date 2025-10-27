//===========================================================================================
// 
//  Author          : Mildred Tosoc
// 
//  File            : Acos3.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Contains Methods and Properties of ACOS3
// 
//  Date            : January 7, 2013
// 
//  Revision Traile : [Author] / [Date of modification] / [Details of Modifications done]
//					  Brey Vaughn Lee/ June 17, 2013    / Updates on change Pin method
// 
//=========================================================================================


#include "stdafx.h"
#include "Acos3.h"
#include "Apdu.h"
#include "CardException.h"
#include "PcscException.h"
#include "AcsReader.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Acos3::Acos3()
{
	pReader_ = NULL;
	pApdu_ = NULL;
}

Acos3::Acos3(AcsReader * prop)
{
	setReader(prop);
}

//////////////////////////////////////////////////////////////////////
// Implementation of accessor functions
//////////////////////////////////////////////////////////////////////
void Acos3::setReader(AcsReader * pReader)
{
	pReader_ = pReader;
}

//////////////////////////////////////////////////////////////////////
// Implementation of other member functions
//////////////////////////////////////////////////////////////////////

void Acos3::clearCard()
{
	if (pReader_ == NULL)
        throw CardException("Connection is not yet established");
	
	byte aResponse[2];
	byte aCommand[5] = { 0x80, 0x30, 0x00, 0x00, 0x00 };	
	byte aStatusWord[2];

	Apdu* pApdu = new Apdu();
	if (pApdu == NULL)
	{
		throw CardException("Unable to Allocate New ADPU");
	}

	pApdu->setCommand(aCommand);	
	pApdu->setSendLen(0x05);
	pApdu->setReceiveLen(0x02);
	pApdu->setReceiveData(aResponse);
	pApdu->setStatusWord(aResponse);
	
	pReader_->sendApduCommand(pApdu);

	memcpy(aStatusWord,pApdu->getStatusWord(),2);
	if(aStatusWord[0] != 0x90)
	{
		char * errorMessage = getErrorMessage(aStatusWord);		
		
		delete pApdu;
		throw CardException(errorMessage ,aStatusWord);
	}

	delete pApdu;
}

void Acos3::submitCode(CODE_TYPE eCodeType,ACOS3_CODE aCode)
{
	if (pReader_ == NULL)
        throw CardException("Connection is not yet established");

    if (aCode == NULL)
        throw CardException("Code has invalid length. Code should be 8 bytes long");

	byte aResponse[2];
	byte aStatusWord[2];
	byte aCommand[5] = { 0x80, 0x20, (byte)eCodeType, 0x00, 0x08 };

	Apdu* pApdu  = new Apdu();
	if (pApdu == NULL)
	{
		throw CardException("Unable to Allocate New ADPU");
	}

 	pApdu->setCommand(aCommand);
	pApdu->setSendData(aCode);	
	pApdu->setSendLen(0x0D);	
	pApdu->setReceiveLen(0x02);
	pApdu->setReceiveData(aResponse);
	pApdu->setStatusWord(aResponse);

	pReader_->sendApduCommand(pApdu);

	memcpy(aStatusWord, pApdu->getStatusWord(),2);
	if(aStatusWord[0] != 0x90)
	{
		char * errorMessage = getErrorMessage(aStatusWord);		
		
		delete pApdu;
		throw CardException(errorMessage ,aStatusWord);
	}

	delete pApdu;
}

void Acos3::changePin(byte* newPin)
{
    if (pReader_ == NULL)
        throw CardException("Connection is not yet established");

    if (newPin == NULL)
        throw CardException("PIN has invalid length. PIN should be 8 bytes long");

	Apdu* pApdu = new Apdu();
	if (pApdu == NULL)
	{
		throw CardException("Unable to Allocate New ADPU");
	}

	byte aResponse[2];
	byte aCommand[5] = { 0x80, 0x24, 0x00, 0x00, 0x08 };	
	byte aStatusWord[2];

    pApdu->setCommand(aCommand);
	pApdu->setSendData(newPin);	
	pApdu->setSendLen(0x0D);
	pApdu->setReceiveLen(0x02);
	pApdu->setReceiveData(aResponse);
	pApdu->setStatusWord(aResponse);

    pReader_->sendApduCommand(pApdu);

	memcpy(aStatusWord,pApdu->getStatusWord(),2);
	if(aStatusWord[0] != 0x90)
	{
		char * errorMessage = getErrorMessage(aStatusWord);		
		
		delete pApdu;
		throw CardException(errorMessage ,aStatusWord);
	}

	delete pApdu;

}

void Acos3::selectFile(INTERNAL_FILE eInternalFile)
{
	byte aFileID[2];
	
	switch (eInternalFile)
	{
		case MCUID_FILE:
		{
			byte fileID[] = { 0xFF, 0x00 };
			memcpy(aFileID, fileID,2);
            break;
		}
		case MANUFACTURER:
        {
            byte fileID[] = { 0xFF, 0x01 };
			memcpy(aFileID, fileID,2);
            break;
        }
        case PERSONALIZATION:
        {
            byte fileID[] = { 0xFF, 0x02 };
			memcpy(aFileID, fileID,2);
            break;
        }
        case SECURITY_FILE:
        {
            byte fileID[] = { 0xFF, 0x03 };
			memcpy(aFileID, fileID,2);
            break;
        }
        case USER_FILE_MGMT_FILE:
        {
            byte fileID[] = { 0xFF, 0x04 };
			memcpy(aFileID, fileID,2);
            break;
        }
        case ACCOUNT_FILE:
        {
            byte fileID[] = { 0xFF, 0x05 };
			memcpy(aFileID, fileID,2);
            break;
        }
        case ACCOUNT_SECURITY:
        {
            byte fileID[] = { 0xFF, 0x06 };
			memcpy(aFileID, fileID,2);
            break;
        }
        case ATR:
        {
            byte fileID[] = { 0xFF, 0x07 };
			memcpy(aFileID, fileID,2);
            break;
        }
        default: throw CardException("Invalid Internal File");
	}
	selectFile(aFileID);
}

void Acos3::selectFile(byte* fileID)
{
	if (pReader_ == NULL)
        throw CardException("Connection is not yet established");

    if (fileID == NULL)
        throw CardException("File ID length is invalid. File ID length should be 2 bytes");

	Apdu* pApdu = new Apdu();
	if (pApdu == NULL)
	{
		throw CardException("Unable to Allocate New ADPU");
	}

	byte statusWord[2];
	byte response[2];
	byte command[5] = { 0x80, 0xA4, 0x00, 0x00, 0x02 };

    pApdu->setCommand(command);
	pApdu->setSendData(fileID);	
	pApdu->setSendLen(2 + 5);
	pApdu->setReceiveLen(0x02);
	pApdu->setReceiveData(response);
	pApdu->setStatusWord(response);


	pReader_->sendApduCommand(pApdu);

	memcpy(statusWord,pApdu->getStatusWord(),2);	
	if(statusWord[0] != 0x90 && statusWord[0] != 0x91)
	{
		char * errorMessage = getErrorMessage(statusWord);		
		
		delete pApdu;
		throw CardException(errorMessage ,statusWord);
	}

	delete pApdu;
}


void Acos3::getResponse(byte lenToReceived,
						byte *cardResponse)
{
	if (pReader_ == NULL)
        throw CardException("Connection is not yet established");

	Apdu* apdu  = new Apdu();
	if (apdu == NULL)
	{
		throw CardException("Unable to Allocate New ADPU");
	}
	
	byte command[5] = {0x80, 0xC0, 0x00, 0x00, lenToReceived};
	byte statusWord[2];

	byte * pResponse = new byte[lenToReceived + 2];
	if (pResponse == NULL)
	{
		delete apdu;
		throw CardException("Unable to Allocate New Read Buffer");
	}

	apdu->setCommand(command);
	apdu->setSendData(NULL);	
	apdu->setSendLen(5);
	apdu->setReceiveLen(lenToReceived + 2);
	apdu->setReceiveData(pResponse);
	apdu->setStatusWord(pResponse + lenToReceived);

	try
	{
		pReader_->sendApduCommand(apdu);

		memcpy(statusWord,apdu->getStatusWord(),2);
		if(statusWord[0] != 0x90)
		{
			char * errorMessage = getErrorMessage(statusWord);		
			
			delete apdu;
			delete [] pResponse;

			throw CardException(errorMessage,statusWord);
		}

		memcpy(cardResponse, apdu->getReceiveData(), lenToReceived);

		delete apdu;
		delete [] pResponse;
	}
	catch (PcscException cException)
	{
		delete apdu;
		delete [] pResponse;
		throw cException;
	}
}



byte* Acos3::getCardInfo(CARD_INFO_TYPE cardInfoType)
{
	byte lenToReceived = 0;
	byte * pResponse;
	byte statusWord[2];

	if (pReader_ == NULL)
        throw CardException("Connection is not yet established");

	Apdu* apdu  = new Apdu();
	if (apdu == NULL)
	{
		throw CardException("Unable to Allocate New ADPU");
	}

	byte command[5] = {0x80, 0x14, 0x00, 0x00, 0x00};

	switch(cardInfoType)
	{
		case CARD_SERIAL:
			{
				command[2] = 0x00;
				command[4] = 0x08;
				lenToReceived = 0x08;
				break;
			}
		case EEPROM:
			{
				command[2] = 0x05;
				command[4] = 0x00;	
				lenToReceived = 0x00;
				break;
			}
		case VERSION_NUMBER:
			{
				command[2] = 0x06;
				command[4] = 0x08;
				lenToReceived = 0x08;
				break;
			}
		default: throw CardException("Invalid card information type");
	}

	pResponse = new byte[lenToReceived + 2];

	apdu->setCommand(command);
	apdu->setSendData(NULL);	
	apdu->setSendLen(0x05);
	apdu->setReceiveLen(lenToReceived + 0x02);
	apdu->setReceiveData(pResponse);

	pReader_->sendApduCommand(apdu);

	if(apdu->getReceiveLen() > 1)
	{
		memcpy(statusWord, apdu->getReceiveData() + (apdu->getReceiveLen() - (int)sizeof(statusWord)), sizeof(statusWord));
	}

	if(statusWord[0] != 0x90)
		throw CardException(getErrorMessage(statusWord), statusWord);
	
	switch(cardInfoType)
	{
		case CARD_SERIAL:
		case VERSION_NUMBER:
			{
				return apdu->getReceiveData();
			}
		case EEPROM:
			{
				return apdu->getStatusWord();
			}
		default: throw CardException("Invalid card information type");
	}
}

int8 * Acos3::getErrorMessage(byte* sw1sw2)
{
	if (sw1sw2 == NULL )
        return "Invalid Parameters";

    else if (sw1sw2[0] == 0x6B && sw1sw2[1] == 0x20)
        return "Amount too large";

    else if (sw1sw2[0] == 0x62 && sw1sw2[1] == 0x81)
        return "Account data may be corrupted";

    else if (sw1sw2[0] == 0x67 && sw1sw2[1] == 0x00)
        return "Specified Length plus offset is larger than record length ";

    else if (sw1sw2[0] == 0x69 && sw1sw2[1] == 0x66)
        return "Command not available; option bit not set";

    else if (sw1sw2[0] == 0x69 && sw1sw2[1] == 0x81)
        return "Command incompatible with file structure";

    else if (sw1sw2[0] == 0x69 && sw1sw2[1] == 0x82)
        return "Security status not satisfied; PIN not submitted prior to issuing this command";

    else if (sw1sw2[0] == 0x69 && sw1sw2[1] == 0x83)
        return "Specified code is locked; Terminal Authentication Key is locked, Authentication process cannot be executed";

    else if (sw1sw2[0] == 0x69 && sw1sw2[1] == 0x85)
        return "No data available; the INQUIRE ACCOUNT command was not executed immediately prior to the GET RESPONSE command; Mutual authentication not successfully completed prior to the SUBMIT Code command; No file selected";

    else if (sw1sw2[0] == 0x69 && sw1sw2[1] == 0xF0)
        return "Data in account is inconsistent – no access unless in Issuer Mode";
        
    else if (sw1sw2[0] == 0x6A && sw1sw2[1] == 0x82)
        return "File does not exist; Account does not exist";

    else if (sw1sw2[0] == 0x6A && sw1sw2[1] == 0x83)
        return "Record not found – file too short";

    else if (sw1sw2[0] == 0x6F && sw1sw2[1] == 0x00)
        return "I/O error; data to be accessed resides in invalid address";

    else if (sw1sw2[0] == 0x6F && sw1sw2[1] == 0x10)
        return "Account Transaction Counter at maximum – no more transaction possible";
	else if (sw1sw2[0] == 0x69 && sw1sw2[1] == 0x88)
        return "MAC does not match the data";
	else if (sw1sw2[0] == 0x6E && sw1sw2[1] == 0x00)
		return "Invalid CLA";

    else if (sw1sw2[0] == 0x63)
    {
		static char retries[100];

		sprintf(retries, "Invalid Pin/key/code; %02X  retries left; MAC cryptographic checksum is wrong ", sw1sw2[1] & 0x0F);
		return retries;    
	}

    else
	{
		static char aStatus[100];
		
		sprintf(aStatus,"Unknown Status Word (%02x%02x)",sw1sw2[0],sw1sw2[1]);

		return aStatus;
	}
        
}

//helper class

void Acos3::configurePersonalizationFile(OptionRegister* optionRegister, SecurityOptionRegister* securityRegister, byte NumberOfFiles)
{
	selectFile(PERSONALIZATION);

	byte data[4] = {optionRegister->getRawValue(), securityRegister->getRawValue(), NumberOfFiles, 0x00 };
	writeRecord(0x00, 0x00, data, 4);
}

void Acos3::createRecordFile(byte recordNumber, byte* fileID, byte numberOfRecords, byte recordLength,
                                     SecurityAttribute* writeSecurityAttribute, SecurityAttribute* readSecurityAttribute,
                                     bool readRequireSecureMessaging, bool writeRequireSecureMessaging)
{
	 byte buffer[7];

     buffer[0] = recordLength;
     buffer[1] = numberOfRecords;
     buffer[2] = readSecurityAttribute->getRawValue();
     buffer[3] = writeSecurityAttribute->getRawValue();
     buffer[4] = fileID[0];
     buffer[5] = fileID[1];


     if (readRequireSecureMessaging)
         buffer[6] |= 0x40;

     if (writeRequireSecureMessaging)
         buffer[6] |= 0x20;

     writeRecord(recordNumber, 0, buffer, 7);
}

void Acos3::submitAppCodes(CODE_TYPE codeToSubmit, byte *code, byte * pResponse)
{
	if (pReader_ == NULL)
		throw CardException("Connection is not yet established");

    if (code == NULL || pResponse == NULL)
        throw CardException("Code has invalid length. Code should be 8 bytes long");

	Apdu* apdu  = new Apdu();
	byte response[2];	

	byte command[5] = { 0x80, 0x20, (byte)codeToSubmit, 0x00, 0x08 };		

 	apdu->setCommand(command);
	apdu->setSendData(code);	
	apdu->setSendLen(0x0D);	
	apdu->setReceiveLen(0x02);
	apdu->setReceiveData(response);
	apdu->setStatusWord(response);

	try 
	{
		pReader_->sendApduCommand(apdu);
	}
	catch  (PcscException pcscException)
	{
		delete apdu;
		throw pcscException;
	}	

	memcpy(pResponse,apdu->getStatusWord(),2);
	delete apdu;
		
}

void Acos3::Acos3DES (BYTE *Data, BYTE *Key, int bEncrypt)
{
	DES (Data, Key, bEncrypt);
}

void Acos3::Acos3DES3 (BYTE *Data, BYTE *Key, int bEncrypt)
{
	DES3 (Data, Key, bEncrypt);
}


