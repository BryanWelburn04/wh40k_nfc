#include "stdafx.h"
#include "AcsReader.h"

AcsReader::~AcsReader()
{
}

void AcsReader::setApduCommand(Apdu* pApduCommand)
{
	pApduCommand_ = pApduCommand;
}

void AcsReader::setSendCommandFunctor(void (*pFunc)(const uint8* data, const int len))
{
	cSendCommandFunctor_.setFunctor(pFunc);
}

void AcsReader::setRecvCommandFunctor(void (*pFunc)(const uint8* data, const int len))
{
	cReceiveCommandFunctor_.setFunctor(pFunc);
}
