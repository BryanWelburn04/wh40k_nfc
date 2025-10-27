// Helper.h: interface for the Helper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELPER_H__BD34FA8E_45F3_440A_9109_711FB5EE1E74__INCLUDED_)
#define AFX_HELPER_H__BD34FA8E_45F3_440A_9109_711FB5EE1E74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AcsCommon.h"

class Helper  
{
public:
	char* byteAsString(const byte* dataBytes, const int byteArrayLength, bool isSpaceInBetween);
	char* byteAsString(const byte* dataBytes, const int startIndex, const int byteArrayLength, bool isSpaceInBetween);
	byte  getByte(char *stringBytes);
	byte* getBytes(char* stringBytes);
	byte* getBytes(char* stringBytes, char delimiter);
	void getBytes(const char *stringBytes, byte *pOutput, uint8 & rOutputLength);
	int32 byteToInt(byte* data);
	byte* intToByte(int number);
	byte* intTo3Byte(int number);
	void uint16ToByte(uint16 number, byte *pByte);
	void byteToUint16(byte *pByte, uint16 &rNumber);
	int32 byteToInt(byte* data,bool isLittleEndian);

	uint32 decimalToUint32(char * pDecimalString);
	void decimalToBytes(char * pDecimalString, byte *pBytes, uint8 uNumBytes);

	bool hexCheck(const char * pString);
	bool decimalCheck(const char * pString);

	string intToHexString(int8 iInput);
	string intToHexString(int16 iInput);
	string intToHexString(int32 iInput);
	string intToDecimalString(int8 iInput);
	string intToDecimalString(int16 iInput);
	string intToDecimalString(int32 iInput);
	string uintToHexString(uint8 uInput);
	string uintToHexString(uint16 uInput);
	string uintToHexString(uint32 uInput);
	string uintToDecimalString(uint8 uInput);
	string uintToDecimalString(uint16 uInput);
	string uintToDecimalString(uint32 uInput);

	void xOr(const byte * pSource, byte * pDest, uint8 uLength);

	Helper();
	virtual ~Helper();

};

/*BIT MACROS*/
#define ISBITSET(x,m)	(((x)&   (m))? TRUE:FALSE)
#define SETBITS(x,m)	( (x)|=  (m))
#define RESETBITS(x,m)	( (x)&= ~(m))
#define GETBITS(x,m)	((x)&(m))

/*8 BIT MACROS*/
#define GET8MSN(x)	((x) >> 4)
#define GET8LSN(x)	((x) & 0x0F)

#define SET8MSN(x,y)	((x) = (((x)&0x0F)|((y)<<4))) 
#define SET8LSN(x,y)	((x) = ( (x)&0xF0)| (y))


/*16 BIT MACROS*/
#define GET16MSB(x)	((x) >> 8)
#define GET16LSB(x)	((x) & 0x00FF)
//x : 16 bit number, y is the 8 bit number
#define SET16MSB(x,y)	((x) = (((x)&0x00FF)|((y)<<8))) 
#define SET16LSB(x,y)	((x) = ( (x)&0xFF00)| (y))

/*32 BIT MACROS*/
#define GET32MSB(x)		( (x) >> 24)
#define GET322NDMSB(x)	(((x) >> 16) & 0x0000FF)
#define GET322NDLSB(x)	(((x) >> 8 ) & 0x0000FF)
#define GET32LSB(x)		( (x)        & 0x0000FF)




#endif // !defined(AFX_HELPER_H__BD34FA8E_45F3_440A_9109_711FB5EE1E74__INCLUDED_)
