/////////////////////////////////////////////////////////////////////////////
// VCardIdentificationType.h : header file
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stdafx.h"
#include <ctime>
#include "Helper.h"
#include "ReferenceData.h"

class VCardIdentificationType
{
public:
	VCardIdentificationType();
	~VCardIdentificationType();

	CString getFormattedName();
    void setFormattedName(CString sFormattedName);

	CString getGivenName();
    void setGivenName(CString sGivenName);

	CString getAdditionalName();
    void setAdditionalName(CString sAdditionalName);

	CString getFamilyName();
    void setFamilyName(CString sFamilyName);

	CString getNickName();
    void setNickName(CString sNickName);

	CString getHonorificPrefix();
    void setHonorificPrefix(CString sHonorificPrefix);

	CString getHonorificSuffix();
    void setHonorificSuffix(CString sHonorificSuffix);

	tm* getBirthDate();
    void setBirthDate(tm* pBirthDate);

	CString getFormattedString();
	void loadData(vector<byte> uData);
	void loadData(CString sDataAsString);

private:
	Helper  cHelper_;
	CString sFormattedName_;
	CString sGivenName_;
	CString sAdditionalName_;
	CString sFamilyName_;
	CString sNickName_;
	CString sHonorificPrefix_;
	CString sHonorificSuffix_;
	tm*		pBirthDate_;
};