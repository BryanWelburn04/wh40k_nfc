#include "stdafx.h"
#include "PcscException.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
PcscException::PcscException()
{
	_message = NULL;
}

PcscException::PcscException(char* errorString)
{
	_message = errorString;
}

PcscException::PcscException(long errorNumber)
{
	
}

PcscException::~PcscException()
{

}

const char* PcscException::what()
{
	return _message;
}


