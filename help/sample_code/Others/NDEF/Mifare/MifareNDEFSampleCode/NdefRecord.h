/////////////////////////////////////////////////////////////////////////////
// NdefRecord.h : header file
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stdafx.h"
#include "NdefRecordType.h"
#include "NdefException.h"
#include "Helper.h"
#include <math.h>

class NdefRecord
{
public:
	// Constructor/Destructor
	NdefRecord();
	NdefRecord(NdefRecordType* pRecordType);
	~NdefRecord();

	bool			getMessageBegin();
	void			setMessageBegin(bool bIsMessageBegin);
	
	bool			getMessageEnd();
	void			setMessageEnd(bool bIsMessageEnd);
	
	bool			getTerminatingRecordChunk();
	void			setTerminatingRecordChunk(bool bIsTerminatingRecordChunk);

	bool			getShortRecord();
	void			setShortRecord(bool bIsShortRecord);

	NdefRecordType* getRecordType();
	void			setRecordType(NdefRecordType* pRecordType);

	vector<byte>	getPayload();
	void			setPayload(vector<byte> uPayload);

	CString			getPayloadString();
	void			setPayloadString(CString sPayload);

	vector<byte>	getMessageId();
	void			setMessageId(vector<byte> uMessageId);

	vector<byte>	encodeToNdef();
	void			appendPayload(vector<byte> uPayload);
	void			appendPayload(CString sPayload);
	void			appendPayload(byte uPayload);

private:
	Helper			cHelper_;
	bool			bIsMessageBegin_;
	bool			bIsMessageEnd_;
	bool			bIsTerminatingRecordChunk_;
	bool			bIsShortRecord_;
	NdefRecordType* pRecordType_;
	vector<byte>	uPayload_;
	vector<byte>	uMessageId_;
};