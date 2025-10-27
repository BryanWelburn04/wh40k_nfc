//===========================================================================================
//  Author         : Teosseth G. Altar
// 
//  File           : Helper.cpp
// 
//  Copyright (C)  : Advanced Card Systems Ltd.
// 
//  Description    : Container class implementation for helper/utility functions
// 
//  Date           : October 18, 2011
// 
//  Revision Trail :
//  Date              Author                  Description
//  04 / 29 / 2014    Maria Czarina Burgos    Added helper functions for vector and 
//                                            CString datatypes
// 
//=========================================================================================

#include "stdafx.h"
#include "Helper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Constructor/Destructor
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
byte* Helper::getBytes(char* pStringBytes)
{
	char		cBuffer[3];
	byte*		pReturnBytes;
	int			iLen = strlen(pStringBytes);

	if (iLen == 0 || iLen % 2 != 0)
		return NULL;

	pReturnBytes = new byte[iLen / 2];

	for (int i = 0, j = 0; i < iLen; i += 2, j++)
	{
		memcpy(cBuffer, pStringBytes + i, 2);
		sscanf(cBuffer, "%x", pReturnBytes + j);
	}

	return pReturnBytes;
}

byte* Helper::getBytes(char* pStringBytes, char cDelimiter)
{
	int			iLen = strlen(pStringBytes);
	char*		pString = new char[iLen];

	if (iLen == 0)
		return NULL;

	for (int i = 0; i < iLen; i++)
	{
		if (pStringBytes[i] != cDelimiter)
			sprintf(pString, "%s%c", pString, pStringBytes[i]);
	}

	return getBytes(pString);
}

byte* Helper::getBytes(CString sStringBytes)
{
	int			 iNum = 0;
	string 		 sString((LPCTSTR)sStringBytes);
	string		 sSend;

	byte* pReturnBytes = new byte[sStringBytes.GetLength() / 2];
    
	for(int i = 0; i < (sStringBytes.GetLength() / 2); i++)
	{
		sSend = sString.substr(i*2, 2);
		stringstream ssSend(sSend);
		ssSend >> hex >> iNum;
		pReturnBytes[i] = (BYTE)iNum;
	}

	return pReturnBytes;
}

// Convert byte array to a hexadecimal-formatted string
char* Helper::byteAsString(byte pDataBytes)
{
	char* 		pReturnString = new char[1];
	
	memset(pReturnString, 0, 1);
	sprintf(pReturnString, "%s%02X ", pReturnString, pDataBytes);

	return pReturnString;
}

char* Helper::byteAsString(const byte* pDataBytes, const int iByteArrayLength, bool isSpaceInBetween)
{
	int			iLen = iByteArrayLength;
	char*		pReturnString = new char[iLen * 3 + 1];
	
	if (iLen == 0)
		return NULL;

	memset (pReturnString, 0, iLen * 2 + 2);

	for (int i = 0; i < iLen; i++)
		sprintf(pReturnString, isSpaceInBetween ? "%s%02X " : "%s%02X", pReturnString, pDataBytes[i]);

	return pReturnString;
}

char* Helper::byteAsString(const byte* pDataBytes, const int iStartIndex, const int iByteArrayLength, bool isSpaceInBetween)
{
	int 		iLen = iByteArrayLength;
	char*		pReturnString = new char[iLen * 3 + 1];
	
	if (iLen == 0)
		return NULL;

	memset (pReturnString, 0, iLen * 2 + 2);

	for (int i = iStartIndex; i < iLen + iStartIndex; i++)
		sprintf(pReturnString, isSpaceInBetween ? "%s%02X " : "%s%02X", pReturnString, pDataBytes[i]);

	return pReturnString;
}

CString Helper::byteAsString(const byte *pDataBytes, const int iByteArrayLength)
{
	CString		sString = "";
	CString		sReturnString = "";

	for(int i = 0; i < iByteArrayLength; i++)
	{
		sString.Format("%02X", pDataBytes[i]);
		sReturnString += sString;
	}

	return sReturnString;
}

// Convert byte array to integer
int Helper::byteToInt(byte* pDataBytes)
{
	int			iReturnValue = 0;

	iReturnValue  = ((int)pDataBytes[0]) << 24;
	iReturnValue |= ((int)pDataBytes[1]) << 16;
	iReturnValue |= ((int)pDataBytes[2]) << 8;
	iReturnValue |= pDataBytes[3];

	return iReturnValue;
}

// Convert integer to byte array
byte* Helper::intToByte(int iValue)
{
	static byte	pReturnBytes[4];

	pReturnBytes[0] = (byte)((iValue >> 24) & 0xFF);
	pReturnBytes[1] = (byte)((iValue >> 16) & 0xFF);
	pReturnBytes[2] = (byte)((iValue >> 8) & 0xFF);
	pReturnBytes[3] = (byte)(iValue& 0xFF);

	return pReturnBytes;
}

byte* Helper::intTo3Byte(int iValue)
{
	static byte pReturnBytes[3];

	pReturnBytes[0] = (byte)((iValue >> 16) & 0xFF);
	pReturnBytes[1] = (byte)((iValue >> 8) & 0xFF);
	pReturnBytes[2] = (byte)(iValue & 0xFF);

	return pReturnBytes;
}

// Append two byte arrays
byte* Helper::appendArrays(byte* pArr1, int iArr1, byte* pArr2, int iArr2)
{
	byte 		*pReturnBytes = new byte[iArr1 + iArr2];
	
	memcpy(pReturnBytes, pArr1, iArr1);
	memcpy(pReturnBytes + iArr1, pArr2, iArr2);

	return pReturnBytes;
}

// Append a byte to an array
byte* Helper::appendArrays(byte* pArr1, int iArr1, byte uData)
{
	byte 		*pReturnBytes = new byte[iArr1 + 1];
	
	memcpy(pReturnBytes, pArr1, iArr1);
	pReturnBytes[iArr1 + 1] = uData;

	return pReturnBytes;
}

// Append an array to a byte
byte* Helper::appendArrays(byte uData, byte* pArr1, int iArr1)
{
	byte 		*pReturnBytes = new byte[1 + iArr1];
	
	pReturnBytes[0] = uData;
	memcpy(pReturnBytes + 1, pArr1, iArr1);

	return pReturnBytes;
}

// Convert byte array to its ASCII equivalent string
CString Helper::bytesToAscii(byte* pDataBytes, const int iByteArrayLength)
{
	if(iByteArrayLength == 0)
		return "";

	CString		sReturnString = "";

	for(int i = 0; i < iByteArrayLength; i++)
	{
		char cData = pDataBytes[i];
		sReturnString += cData;
	}
	return sReturnString;
}

// Convert ASCII string to byte array
byte* Helper::asciiToBytes(CString sData)
{
	int 		iLen = sData.GetLength();
	byte* 		pReturnBytes;
	
	if (iLen == 0)
		return NULL;

	pReturnBytes = new byte[iLen];

	for (int i = 0; i < iLen; i++)
	{
		pReturnBytes[i] = sData[i];
	}

	return pReturnBytes;
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

    if (cCharacters.GetLength() < 1)
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
