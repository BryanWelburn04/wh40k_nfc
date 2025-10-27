// Helper.h: interface for the Helper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELPER_H__BD34FA8E_45F3_440A_9109_711FB5EE1E74__INCLUDED_)
#define AFX_HELPER_H__BD34FA8E_45F3_440A_9109_711FB5EE1E74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ACSCommon.h"

class Helper  
{
public:
	char* byteAsString(const uint8* dataBytes, const int byteArrayLength, bool isSpaceInBetween);
	char* byteAsString(const uint8* dataBytes, const int startIndex, const int byteArrayLength, bool isSpaceInBetween);
	uint8* getBytes(char* stringBytes);
	uint8* getBytes(char* stringBytes, char delimiter);
	int byteToInt(uint8* data);
	uint8* intToByte(int number);
	uint8* intTo3Byte(int number);
	int byteToInt(uint8* data,bool isLittleEndian);
	Helper();
	virtual ~Helper();

};

#endif // !defined(AFX_HELPER_H__BD34FA8E_45F3_440A_9109_711FB5EE1E74__INCLUDED_)
