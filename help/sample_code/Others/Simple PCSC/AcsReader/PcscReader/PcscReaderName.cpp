#include "stdafx.h"

#include <string.h>
#include "PcscReaderName.h"
#include "PcscException.h"

PcscReaderName::PcscReaderName()
{
	pReaderName_ = NULL;
	uReaderNameLen_ = 0;
}

PcscReaderName::PcscReaderName(int8 *pNameStr)
{
	if (pNameStr == NULL)
	{
		pReaderName_ = NULL;
		uReaderNameLen_ = 0;
		return;
	}

	uReaderNameLen_ = strlen(pNameStr) + 1;

	pReaderName_ = new int8[uReaderNameLen_];
	if (pReaderName_ == NULL)
	{
		pReaderName_ = NULL;
		uReaderNameLen_ = 0;
		return;
	}

	memcpy(pReaderName_,pNameStr,uReaderNameLen_);

}

PcscReaderName::~PcscReaderName()
{
	if (pReaderName_ != NULL)
	{
		delete [] pReaderName_;
		pReaderName_ = NULL;
		uReaderNameLen_ = 0;
	}
	
}

void PcscReaderName::setNameBuffer(int8 *pNameBuffer)
{
	int8 * pTempNameBuffer;
	if (pNameBuffer == NULL)
	{
		throw PcscException("Invalid Parameter");
	}	

	pTempNameBuffer = new int8 [strlen(pNameBuffer) + 1];
	if (pTempNameBuffer == NULL)
	{
		throw PcscException("Insufficient Memory");
	}

	if (pReaderName_ != NULL)
	{
		delete [] pReaderName_;
	}

	pReaderName_ = pTempNameBuffer;
	uReaderNameLen_ = strlen(pNameBuffer) + 1;
	memcpy(pReaderName_, pNameBuffer, uReaderNameLen_);	
}

int8 * PcscReaderName::getNameBuffer()
{
	return pReaderName_;
}

uint32 PcscReaderName::getNameLength()
{
	return uReaderNameLen_;
}

