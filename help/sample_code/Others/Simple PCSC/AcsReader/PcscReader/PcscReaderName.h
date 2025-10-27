#pragma once

#include "AcsCommon.h"

class PcscReaderName
{
private:
	int8 * pReaderName_;
	uint32 uReaderNameLen_;
public:
	int8 * getNameBuffer();
	uint32 getNameLength();

	void setNameBuffer(int8 *pNameBuffer);

	PcscReaderName();
	PcscReaderName(int8 *pNameStr);
	
	~PcscReaderName();
};