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

private:
	Helper  helper_;
	CString sFormattedName_;
	CString sGivenName_;
	CString sAdditionalName_;
	CString sFamilyName_;
	CString sNickName_;
	CString sHonorificPrefix_;
	CString sHonorificSuffix_;
	tm*		pBirthDate_;
};