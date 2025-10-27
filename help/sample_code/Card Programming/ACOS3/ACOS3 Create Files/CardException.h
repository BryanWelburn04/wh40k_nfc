#pragma once

#include "PcscReader.h"
#include "Apdu.h"
#include "Helper.h"	

class CardException
{
public:
	// accessor functions
	byte* getStatusWord() { return _statusWord; }
	char* getMessage() const { return _message; }

	CardException();
	CardException(char* message);
	CardException(char* message,byte* sw);
	CardException::CardException(const CardException & rhs);
	CardException::~CardException();

private:
	byte  _statusWord[2];
    char* _message;
	
};
