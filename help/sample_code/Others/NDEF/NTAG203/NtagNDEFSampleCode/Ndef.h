/////////////////////////////////////////////////////////////////////////////
// Ndef.h : header file
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stdafx.h"
#include "Helper.h"
#include "NdefMessage.h"
#include "ReferenceData.h"

class Ndef
{
public:
	Ndef();  // Constructor
	~Ndef(); // Destructor

	NdefMessage* getNestedNdefMessage(NdefRecord* pNdefRecord);
	NdefMessage* getNestedNdefMessage(int iIndex, vector<byte> uData);
	CString		 decodeNdefMessage(vector<byte> uData);
	vector<byte> encodeSmartPoster(CString sTitleLanguage, CString sTitle, URI_IDENTIFIER_CODE eUriPrefix, CString sUri, ACTION_RECORD eAction);
	vector<byte> encodeNfcForumExternalType(CString sType, CString sPayload);
	CString		 getURIIdentifierCode(URI_IDENTIFIER_CODE eCode);
	CString		 getActionCode(ACTION_RECORD eCode);
	bool		 isTelUriValid(CString sUri);

private:
	Helper cHelper_;
	list<NdefRecord> ndefRecords_;
};