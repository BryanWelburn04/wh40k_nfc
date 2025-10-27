//===========================================================================================
// 
//  Author          : Teosseth G. Altar
// 
//  File            : Helper.cpp:
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Container class implementation for helper/utility functions
// 
//  Date            : October 18, 2011
// 
//  Revision Traile : [Author] / [Date of modification] / [Details of Modifications done]
// 
// 
//=========================================================================================
// Helper.cpp: implementation of the Helper class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ACSCommon.h"
#include "Helper.h"

#include <iostream>

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Helper::Helper()
{
 
}

Helper::~Helper()
{

}


//////////////////////////////////////////////////////////////////////
// Implementation of member functions
//////////////////////////////////////////////////////////////////////

// Convert hexadecimal-formatted string to a byte array
// Overloaded version
byte Helper::getByte(char *stringBytes)
{
	char buffer[3];
	byte returnByte;
	int iResult;
	int len = strlen(stringBytes);

	if (len == 0 || len > 2)		
	{
		throw "Invalid String Length";
	}

	memcpy(buffer, stringBytes, 2);		
	sscanf(buffer, "%x", &iResult);
	returnByte = iResult;

	return returnByte;
}

byte* Helper::getBytes(char *stringBytes, char delimiter)
{
	int len = strlen(stringBytes);
	char* string = new char[len];

	if (len == 0)
		return NULL;

	for (int i = 0; i < len; i++)
	{
		if (stringBytes[i] != delimiter)
			sprintf(string, "%s%c", string, stringBytes[i]);
	}


	return getBytes(string);
}

// Convert hexadecimal-formatted string to a byte array
byte* Helper::getBytes(char *stringBytes)
{
	char buffer[3];
	byte* returnBytes;
	int len = strlen(stringBytes);

	if (len == 0 || len % 2 != 0)
		return NULL;

	returnBytes = new byte[len / 2];

	for (int i = 0, j = 0; i < len; i += 2, j++)
	{
		int iResult;
		memcpy(buffer, stringBytes + i, 2);		
		sscanf(buffer, "%x", &iResult);
		returnBytes[j] = iResult;
	}

	return returnBytes;
}

void Helper::getBytes(const char *stringBytes, byte *pOutput, uint8 & rOutputLength)
{
	char buffer[3];
	
	int len = strlen(stringBytes);

	if (len == 0 || len % 2 != 0)		
	{
		throw "Invalid String Length";
	}

	if (len / 2 > rOutputLength)
	{
		throw "Insufficient Output Length";
	}

	

	for (int i = 0, j = 0; i < len; i += 2, j++)
	{
		int iResult;
		memcpy(buffer, stringBytes + i, 2);		
		sscanf(buffer, "%02x", &iResult);

		pOutput[j] = iResult;
	}	
}

bool Helper::decimalCheck(const char * pString)
{
	int iIdx;

	if (pString == NULL)
	{
		return false;
	}

	if (strlen(pString) == 0)
	{
		return false;
	}

	for (iIdx = 0; iIdx < strlen(pString); iIdx ++)
	{
		switch (pString[iIdx])
		{
			case '0': case '1': case '2': case '3':	case '4':
			case '5': case '6':	case '7': case '8':	case '9':			
				break;
			default:
				return false;
		}
	}

	return true;
}

bool Helper::hexCheck(const char * pString)
{
	int iIdx;

	if (pString == NULL)
	{
		return false;
	}

	if (strlen(pString) == 0)
	{
		return false;
	}

	for (iIdx = 0; iIdx < strlen(pString); iIdx ++)
	{
		switch (pString[iIdx])
		{
			case '0': case '1': case '2': case '3':	case '4':
			case '5': case '6':	case '7': case '8':	case '9':
			case 'a': case 'b':	case 'c': case 'd':	case 'e': case 'f':
			case 'A': case 'B':	case 'C': case 'D':	case 'E': case 'F':
				break;
			default:
				return false;
		}
	}

	return true;
}

// Convert byte array to a hexadecimal-formatted string
char* Helper::byteAsString(const byte *dataBytes, const int byteArrayLength, bool isSpaceInBetween)
{
	int len = byteArrayLength;
	char* returnString = new char[len * 3 + 1];
	
	if (len == 0)
		return NULL;

	memset (returnString, 0, len * 2 + 2);

	for (int i = 0; i < len; i++)
		sprintf(returnString, isSpaceInBetween ? "%s%02X " : "%s%02X", returnString, dataBytes[i]);

	return returnString;
}

// Convert byte array to a hexadecimal-formatted string
char* Helper::byteAsString(const byte *dataBytes, const int startIndex, int byteArrayLength, bool isSpaceInBetween)
{
	int len = byteArrayLength;
	char* returnString = new char[len * 3 + 1];
	
	if (len == 0)
		return NULL;

	memset (returnString, 0, len * 2 + 2);

	for (int i = startIndex; i < len + startIndex; i++)
		sprintf(returnString, isSpaceInBetween ? "%s%02X " : "%s%02X", returnString, dataBytes[i]);

	return returnString;
}

//convert byte array to integer
int Helper::byteToInt(byte* data)
{
	int tmpInt = 0;

	tmpInt = ((int)data[0]) << 24;
	tmpInt |= ((int)data[1]) << 16;
	tmpInt |= ((int)data[2]) << 8;
	tmpInt |= data[3];

	return tmpInt;
}

//convert integer to byte array
byte* Helper::intToByte(int number)
{
	static byte tmpByte[4];

	tmpByte[0] = (byte)((number >> 24) & 0xFF);
	tmpByte[1] = (byte)((number >> 16) & 0xFF);
	tmpByte[2] = (byte)((number >> 8) & 0xFF);
	tmpByte[3] = (byte)(number& 0xFF);

	return tmpByte;
}

void Helper::decimalToBytes(char * pDecimalString, byte *pBytes, uint8 uNumBytes)
{
	if (pDecimalString == NULL || pBytes == NULL || uNumBytes != 4)
		throw "Invalid Parameters";

	int iResult;

	sscanf(pDecimalString,"%d",&iResult);

	byte * pResult;

	pResult = intToByte(iResult);

	memcpy(pBytes,pResult,4);	
}

unsigned int Helper::decimalToUint32(char * pDecimalString)
{
	if (pDecimalString == NULL)
		throw "Invalid Parameter";

	unsigned int uResult;

	sscanf(pDecimalString,"%u",&uResult);

	return uResult;
}

void Helper::uint16ToByte(uint16 number, byte *pByte)
{		
	if (pByte == NULL)
	{
		throw "Null Parameter Invalid.";
	}
	pByte[0] = (byte)((number >> 8) & 0xFF);
	pByte[1] = (byte)(number& 0xFF);

}

void Helper::byteToUint16(byte *pByte, uint16 &rNumber)
{		
	if (pByte == NULL)
	{
		throw "Null Parameter Invalid.";
	}

	rNumber = (pByte[0]<<8) | pByte[1];	

}

//convert integer to byte array
byte* Helper::intTo3Byte(int number)
{
	static byte tmpByte[3];

	tmpByte[0] = (byte)((number >> 16) & 0xFF);
	tmpByte[1] = (byte)((number >> 8) & 0xFF);
	tmpByte[2] = (byte)(number & 0xFF);

	return tmpByte;
}

void Helper::xOr(const byte * pSource, byte * pDest, uint8 uLength)
{
	if (pSource == NULL || pDest == NULL)
	{
		throw "Invalid Parameters";
	}

	int iIdx = 0;

	for (iIdx  = 0; iIdx < uLength; iIdx ++)
	{
		pDest[iIdx] = pSource[iIdx] ^ 0xFF; 
	}
}

string Helper::intToHexString(int8 iInput)
{
	int8 aData[20];
	sprintf(aData,"%02X",iInput);
	return aData;
}

string Helper::intToHexString(int16 iInput)
{
	int8 aData[20];
	sprintf(aData,"%04X",iInput);
	return aData;
}
string Helper::intToHexString(int32 iInput)
{
	int8 aData[20];
	sprintf(aData,"%08X",iInput);
	return aData;
}

string Helper::intToDecimalString(int8 iInput)
{
	int8 aData[20];
	sprintf(aData,"%d",iInput);
	return aData;
}

string Helper::intToDecimalString(int16 iInput)
{
	int8 aData[20];
	sprintf(aData,"%d",iInput);
	return aData;
}

string Helper::intToDecimalString(int32 iInput)
{
	int8 aData[20];
	sprintf(aData,"%d",iInput);
	return aData;
}

string Helper::uintToHexString(uint8 uInput)
{
	int8 aData[20];
	sprintf(aData,"%02X",uInput);
	return aData;
}

string Helper::uintToHexString(uint16 uInput)
{
	int8 aData[20];
	sprintf(aData,"%04X",uInput);
	return aData;
}

string Helper::uintToHexString(uint32 uInput)
{
	int8 aData[20];
	sprintf(aData,"%08X",uInput);
	return aData;
}

string Helper::uintToDecimalString(uint8 uInput)
{
	int8 aData[20];
	sprintf(aData,"%d",uInput);
	return aData;
}

string Helper::uintToDecimalString(uint16 uInput)
{
	int8 aData[20];
	sprintf(aData,"%d",uInput);
	return aData;
}

string Helper::uintToDecimalString(uint32 uInput)
{
	int8 aData[20];
	sprintf(aData,"%d",uInput);
	return aData;
}