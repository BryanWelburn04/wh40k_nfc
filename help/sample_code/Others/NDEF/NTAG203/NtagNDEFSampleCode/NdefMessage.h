/////////////////////////////////////////////////////////////////////////////
// NdefMessage.h : header file
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stdafx.h"
#include "Helper.h"
#include "NdefRecord.h"
#include <list>

class NdefMessage
{
public:
	NdefMessage();  // Constructor
	~NdefMessage(); // Destructor

	void appendRecord(NdefRecord cRecord);
	void insertRecord(int iIndex, NdefRecord cRecord);
	list<NdefRecord> getRecords();
	NdefRecord getRecord(NdefRecordType cRecordType);
	int getNumberOfRecords();
	vector<byte> toByteArray();

private:
	Helper cHelper_;
	list<NdefRecord> ndefRecords_;
};