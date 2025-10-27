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
#include "ACOS Sample Codes.h"
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
		memcpy(buffer, stringBytes + i, 2);
		sscanf(buffer, "%x", returnBytes + j);
	}

	return returnBytes;
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
	byte tmpByte[4];

	tmpByte[0] = (byte)((number >> 24) & 0xFF);
	tmpByte[1] = (byte)((number >> 16) & 0xFF);
	tmpByte[2] = (byte)((number >> 8) & 0xFF);
	tmpByte[3] = (byte)(number& 0xFF);

	return tmpByte;
}


