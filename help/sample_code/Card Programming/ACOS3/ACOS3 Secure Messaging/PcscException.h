#pragma once

#include "AcsCommon.h"

class PcscException
{
public:	
	virtual const char* what();	

	PcscException();
	PcscException(long errorNumber);
	PcscException(char* errorString);
	virtual ~PcscException();
protected:
	const char* _message;
};