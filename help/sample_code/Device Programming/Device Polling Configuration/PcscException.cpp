#include "stdafx.h"
#include "PcscException.h"

PcscException::PcscException()
{
	iErrorCode_ = 0;
    pMessage_ = NULL;
}

PcscException::PcscException(char* pMessage)
{
	iErrorCode_ = 0;
    pMessage_ = pMessage;
}
PcscException::PcscException(int64 iErrorCode, char* pMessage)
{
	iErrorCode_ = iErrorCode;
	pMessage_ = pMessage;
}

PcscException::~PcscException()
{
	iErrorCode_ = 0;
	pMessage_ = "";
}
