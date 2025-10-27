/////////////////////////////////////////////////////////////////////////////
// Ndef.h : header file
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stdafx.h"
#include "Helper.h"
#include "NdefMessage.h"
#include "ReferenceData.h"

struct NdefMessageInformation
{
public:
	TYPE_NAME_FORMAT eTypeNameFormat;
	char* pTypeName;
	bool bMessageBegin;
	bool bMessageEnd;
	bool bIsTerminatingRecordChunk;
	byte* pPayLoad;
	byte uPayLoadLength;
	CString sPayLoadStr;
	byte* pMessageId;
};

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
	bool		 isNameValid(CString sName);

private:
	Helper cHelper_;
	list<NdefRecord> ndefRecords_;
};