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

boolean Helper::decimalCheck(const char * pString)
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

boolean Helper::hexCheck(const char * pString)
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

long Helper::byteToInt(byte* data, long dataLength)
{
	byte* tmpData = NULL;
	byte* tmpContainer = NULL;

	tmpData = new byte[dataLength];
	memset(tmpData, 0x00, dataLength);
	
	memcpy(tmpData, data, dataLength);

	byte* arr = tmpData;

	if (dataLength != 4)
	{
		arr = ReverseArray(arr, dataLength);
		tmpData = new byte[4];
		memset(tmpData, 0x00, 4);
		memcpy(tmpData, arr, 4);

		arr = ReverseArray(tmpData, 4);
	}

	return (arr[0] << 24) + (arr[1] << 16) + (arr[2] << 8) + arr[3];
}

byte* Helper::ReverseArray(byte* orig,long b)
{
    unsigned short int a=0;
    int swap;
   
    for(a;a<--b;a++) //increment a and decrement b until they meet eachother
    {
        swap=orig[a];       //put what's in a into swap space
        orig[a]=orig[b];    //put what's in b into a
        orig[b]=swap;       //put what's in the swap (a) into b
    }
   
    return orig;    //return the new (reversed) string (a pointer to it)
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

// Convert byte array to a hexadecimal-formatted string
char* Helper::byteAsString(byte pDataBytes)
{
	char* 		pReturnString = new char[1];
	
	memset(pReturnString, 0, 1);
	sprintf(pReturnString, "%s%02X ", pReturnString, pDataBytes);

	return pReturnString;
}

// Insert escape characters
CString Helper::escapeString(CString sData)
{
	return escapeString(sData, ",;");
}

CString Helper::escapeString(CString sData, CString cCharacters)
{
	CString		sParsedString = "";
    CString		sTempString = "";

    if (sData == "")
        return sData;

    if (cCharacters == "" || cCharacters.GetLength() < 1)
        return sData;

    sParsedString = sData;

    for (int i = 0; i < cCharacters.GetLength(); i++)
    {
        sTempString = sParsedString;

        for (int iIdx = 0; iIdx < sTempString.GetLength();)
        {
         	if (sTempString[iIdx] == cCharacters[i])
         	{
         		sTempString.Insert(iIdx, "\\");
         		iIdx += 2;
         	}
            else
            {
                iIdx++;
            }
        }

        sParsedString = sTempString;
    }

    return sParsedString;
}

// Removes whitespaces from a string
char* Helper::trim(char* pString)
{
    char *pEnd = pString + strlen(pString) - 1;
    
	while(*pString && isspace(*pString))
        *pString++ = 0;

    while(isspace(*pEnd))
        *pEnd-- = 0;

    return pString;
}

// Convert a vector of byte datatype to its equivalent integer number
int Helper::vectorToInt(vector<byte> uData)
{
	int			iReturnValue = 0;

	iReturnValue  = ((int)uData.at(0)) << 24;
	iReturnValue |= ((int)uData.at(1)) << 16;
	iReturnValue |= ((int)uData.at(2)) << 8;
	iReturnValue |= uData.at(3);

	return iReturnValue;
}

// Convert an integer number to a vector of byte datatype
vector<byte> Helper::intToVector(int iValue)
{
	vector<byte> uReturnBytes;

	uReturnBytes.push_back((byte)((iValue >> 24) & 0xFF));
	uReturnBytes.push_back((byte)((iValue >> 16) & 0xFF));
	uReturnBytes.push_back((byte)((iValue >> 8) & 0xFF));
	uReturnBytes.push_back((byte)(iValue& 0xFF));

	return uReturnBytes;
}

// Convert hexadecimal-formatted string to a vector of byte datatype
CString Helper::vectorToString(vector<byte> uDataBytes)
{
	CString		sReturnString = "";
	CString		sString = "";

	for(int i = 0; i < uDataBytes.size(); i++)
	{
		sString.Format("%02X", uDataBytes.at(i));
		sReturnString += sString;
	}

	return sReturnString;
}

vector<byte> Helper::stringToVector(CString sData)
{
	vector<byte> uDataBytes;

	for(int i = 0; i < sData.GetLength(); i++)
		uDataBytes.push_back(sData[i]);

	return uDataBytes;
}

// Convert a vector of byte datatype to its ASCII equivalent string
CString Helper::vectorToAscii(vector<byte> uDataBytes)
{
	if(uDataBytes.size() == 0)
		return "";

	CString sReturnString = "";

	for(int i = 0; i < uDataBytes.size(); i++)
	{
		char cData = uDataBytes.at(i);
		sReturnString += cData;
	}
	return sReturnString;
}

// Convert ASCII string to a vector of byte datatype
vector<byte> Helper::asciiToVector(CString sData)
{
	int			iLen = sData.GetLength();
	vector<byte> uReturnBytes;
	
	if (iLen == 0)
		return uReturnBytes;

	for (int i = 0; i < iLen; i++)
		uReturnBytes.push_back(sData[i]);

	return uReturnBytes;
}

//MessageBoxes
void Helper::showInformationMessage(CString sMessage)
{
	::MessageBox(0, sMessage, "Mifare NDEF Sample Code", MB_OK|MB_ICONINFORMATION);
}

void Helper::showWarningMessage(CString sMessage)
{
	::MessageBox(0, sMessage, "Mifare NDEF Sample Code", MB_OK|MB_ICONEXCLAMATION);
}

void Helper::showErrorMessage(CString sMessage)
{
	::MessageBox(0, sMessage, "Error", MB_OK|MB_ICONSTOP);
}
