#pragma once

#pragma warning (disable:4786)

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <vector>


using namespace std;

//type definitions
typedef unsigned char	byte;
typedef unsigned char	boolean;

typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned int	uint32; //DWORD
typedef unsigned long	uint64; // not used in windows still 32 bits

typedef char			int8;
typedef short			int16;
typedef int				int32;
typedef long			int64; // not used in windows still 32 bits

typedef float			float32;
typedef double			float64; //same with long double

typedef vector<byte>   ByteArray;
typedef vector<string> StringVector;
typedef list<string>   StringList;

typedef int32 (*AcsCallback) (void * pParameters);

enum ACSTAT
{
	ACSTAT_OK = 0,
	ACSTAT_PENDING,
	ACSTAT_INVALID_PARAMETERS,
	ACSTAT_FAIL
};

