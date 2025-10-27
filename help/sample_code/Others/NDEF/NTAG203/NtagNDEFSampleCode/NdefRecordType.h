/////////////////////////////////////////////////////////////////////////////
// NdefRecordType.h : header file
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "NdefRecordType.h"
#include "ReferenceData.h"

class NdefRecordType
{
public:
	// Constructor/Destructor
	NdefRecordType();
	NdefRecordType(TYPE_NAME_FORMAT eTypeNameFormat, CString sName);
	~NdefRecordType();

	TYPE_NAME_FORMAT getTypeNameFormat() const { return eTypeNameFormat_; }
	void setTypeNameFormat(TYPE_NAME_FORMAT eTypeNameFormat);
	CString getTypeName() const { return sTypeName_; }
	void setTypeName(CString sName);

private:
	TYPE_NAME_FORMAT eTypeNameFormat_;
	CString			 sTypeName_;
};