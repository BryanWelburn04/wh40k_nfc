//////////////////////////////////////////////////////////////////////
// PICCClass.h: interface for the PICCClass
//////////////////////////////////////////////////////////////////////

#pragma once
#include "PcscReader.h"

class PICCClass : public PcscReader
{
private:
	Apdu* pApdu_;

public:
	// Constructor
	PICCClass();
	~PICCClass();

	// Global Variable
	Helper helper_;

	// Functions
	void getData(byte uIso14443A, byte *pResponse, int &iLength);
	void sendCommand(int iCaseType, byte uCla, byte uIns, byte uP1, byte uP2, byte uLc, byte uLe, byte* uData);

private:
	char* getErrorMessage(byte* pStatusWord);
};