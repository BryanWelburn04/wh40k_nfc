// Helper.h: interface for the Helper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELPER_H__BD34FA8E_45F3_440A_9109_711FB5EE1E74__INCLUDED_)
#define AFX_HELPER_H__BD34FA8E_45F3_440A_9109_711FB5EE1E74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Helper  
{
public:
	char* byteAsString(const byte* dataBytes, const int byteArrayLength, bool isSpaceInBetween);
	char* byteAsString(const byte* dataBytes, const int startIndex, const int byteArrayLength, bool isSpaceInBetween);
	byte* getBytes(char* stringBytes);
	byte* getBytes(char* stringBytes, char delimiter);
	int byteToInt(byte* data);
	byte* intToByte(int number);
	byte* intTo3Byte(int number);
	int byteToInt(byte* data,bool isLittleEndian);
	bool checkIfValidHexCharacter(char iCharacter);
	Helper();
	virtual ~Helper();

};

#endif // !defined(AFX_HELPER_H__BD34FA8E_45F3_440A_9109_711FB5EE1E74__INCLUDED_)
