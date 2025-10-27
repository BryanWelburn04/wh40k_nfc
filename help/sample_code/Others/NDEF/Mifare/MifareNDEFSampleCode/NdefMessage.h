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

	void appendRecord(NdefRecord cNdefRecord);
	void insertRecord(int iIndex, NdefRecord cNdefRecord);
	list<NdefRecord> getRecords();
	NdefRecord getRecord(NdefRecordType cNdefRecordType);
	int getNumberOfRecords();
	vector<byte> toByteArray();

private:
	list<NdefRecord> ndefRecords_;
};