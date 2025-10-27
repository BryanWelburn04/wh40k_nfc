//////////////////////////////////////////////////////////////////////
// Helper.h: interface for the Helper class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELPER_H__BD34FA8E_45F3_440A_9109_711FB5EE1E74__INCLUDED_)
#define AFX_HELPER_H__BD34FA8E_45F3_440A_9109_711FB5EE1E74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "string.h"
#include <vector>
#include <sstream>

using namespace std;

class Helper  
{
public:
	Helper();		   // Constructor
	virtual ~Helper(); // Destructor

	byte*		 getBytes(char* pStringBytes);
	byte*		 getBytes(char* pStringBytes, char cDelimiter);
	byte*		 getBytes(CString sStringBytes);
	char*		 byteAsString(byte pDataBytes);
	char*		 byteAsString(const byte* pDataBytes, const int iByteArrayLength, bool isSpaceInBetween);
	char*		 byteAsString(const byte* pDataBytes, const int iStartIndex, const int iByteArrayLength, bool isSpaceInBetween);
	CString		 byteAsString(const byte *pDataBytes, const int iByteArrayLength);
	int			 byteToInt(byte* pDataBytes);
	byte*		 intToByte(int iValue);
	byte*		 intTo3Byte(int iValue);
	byte*		 appendArrays(byte* pArr1, int iArr1, byte* pArr2, int iArr2);
	byte*		 appendArrays(byte* pArr1, int iArr1, byte uData);
	byte*		 appendArrays(byte uData, byte* pArr1, int iArr1);
	CString 	 bytesToAscii(byte* pDataBytes, const int iByteArrayLength);
	byte*		 asciiToBytes(CString sData);
	CString 	 escapeString(CString sData);
	CString 	 escapeString(CString sData, CString cCharacters);
	char*		 trim(char* pString);
	int			 vectorToInt(vector<byte> uData);
	vector<byte> intToVector(int iValue);
	CString		 vectorToString(vector<byte> uDataBytes);
	vector<byte> stringToVector(CString sData);
	CString		 vectorToAscii(vector<byte> uDataBytes);
	vector<byte> asciiToVector(CString sData);
};

/*BIT MACROS*/
#define ISBITSET(x,m)	(((x)&   (m))? TRUE:FALSE)
#define SETBITS(x,m)	( (x)|=  (m))
#define RESETBITS(x,m)	( (x)&= ~(m))
#define GETBITS(x,m)	((x)&(m))

#endif // !defined(AFX_HELPER_H__BD34FA8E_45F3_440A_9109_711FB5EE1E74__INCLUDED_)
