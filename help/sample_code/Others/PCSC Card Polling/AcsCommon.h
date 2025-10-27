#pragma once

#pragma warning (disable:4786)

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <vector>

using namespace std;

#define TRUE	1
#define FALSE	0

typedef unsigned char	boolean;
typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned long	uint32; 
typedef unsigned long	uint64; // not used in windows still 32 bits
//typedef unsigned int	uint32; //DWORD, long
typedef char			int8;
typedef short			int16;
typedef int				int32;
typedef float			float32;
typedef double			float64; //same with long double

typedef vector<byte>   ByteArray;
typedef vector<string> StringVector;
typedef list<string>   StringList;


enum ACSTAT
{
	ACSTAT_OK = 0,
	ACSTAT_PENDING,
	ACSTAT_INVALID_PARAMETERS,
	ACSTAT_FAIL
};

/*BIT MACROS*/
#define ISBITSET(x,m)	(((x) & (m))? TRUE:FALSE)
#define SETBITS(x,m)	( (x) |= (m))
#define RESETBITS(x,m)	( (x) &= ~(m))

/*8 BIT MACROS*/
#define GET8MSN(x)	((x) >> 4)
#define GET8LSN(x)	((x) & 0x0F)

/*16 BIT MACROS*/
#define GET16MSB(x)	((x) >> 8)
#define GET16LSB(x)	((x) & 0x00FF)
//x : 16 bit number, y is the 8 bit number
#define SET16MSB(x,y)	((x) = (((x) & 0x00FF) | ((y)<<8))) 
#define SET16LSB(x,y)	((x) = ( (x) & 0xFF00) |  (y)     )

//32 BIT MACROS
#define GET32MSWORD(x)	((x) >> 16)
#define GET32LSWORD(x)	((x) & 0xFFFF)

