#include "stdafx.h"
#include "AcsReader.h"


AcsReader::AcsReader()
{
	uAtrLen_ = CFG_ATR_BUFFER_LEN;
}

AcsReader::~AcsReader()
{
}

void AcsReader::setApduCommand(Apdu* pApduCommand)
{
	pApduCommand_ = pApduCommand;
}

void AcsReader::setSendCommandFunctor(void (*pFunc)(const byte* data, const int len))
{
	cSendCommandFunctor_.setFunctor(pFunc);
}

void AcsReader::setReceiveCommandFunctor(void (*pFunc)(const byte* data, const int len))
{
	cReceiveCommandFunctor_.setFunctor(pFunc);
}
