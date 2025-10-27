#pragma once
#include "AcsReaderException.h"

class PcscException : public AcsReaderException
{
public:
	const char* what();
	PcscException ();
	PcscException (long errorNumber);
	PcscException (char* errorString);
	virtual ~PcscException ();

private:
	char* getErrorMsg(long errCode);

private:	
	char _errString[50];
};