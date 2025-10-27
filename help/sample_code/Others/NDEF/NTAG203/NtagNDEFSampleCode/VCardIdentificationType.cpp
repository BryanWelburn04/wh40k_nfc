//=========================================================================================
//  Author          : Maria Czarina V. Burgos
// 
//  File            : VCardIdentificationType.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Class implementation for handling VCard Identification Types
// 
//  Date            : April 25, 2014
// 
//  Revision Trail :
//  Date              Author                  Description
// 
//=========================================================================================

#include "stdafx.h"
#include "VCardIdentificationType.h"

//////////////////////////////////////////////////////////////////////
// Constructor/Destructor
//////////////////////////////////////////////////////////////////////
VCardIdentificationType::VCardIdentificationType()
{
	sFormattedName_ = "";
	sGivenName_ = "";
	sAdditionalName_ = "";
	sFamilyName_ = "";
	sNickName_ = "";
	sHonorificPrefix_ = "";
	sHonorificSuffix_ = "";
	pBirthDate_ = NULL;
}

VCardIdentificationType::~VCardIdentificationType()
{

}

//////////////////////////////////////////////////////////////////////
// Implementation of member functions
//////////////////////////////////////////////////////////////////////
CString VCardIdentificationType::getFormattedName()
{
	return sFormattedName_;
}

void VCardIdentificationType::setFormattedName(CString sFormattedName)
{
	sFormattedName_ = sFormattedName;
}

CString VCardIdentificationType::getGivenName()
{
	return sGivenName_;
}

void VCardIdentificationType::setGivenName(CString sGivenName)
{
	sGivenName_ = sGivenName;
}

CString VCardIdentificationType::getAdditionalName()
{
	return sAdditionalName_;
}

void VCardIdentificationType::setAdditionalName(CString sAdditionalName)
{
	sAdditionalName_ = sAdditionalName;
}

CString VCardIdentificationType::getFamilyName()
{
	return sFamilyName_;
}

void VCardIdentificationType::setFamilyName(CString sFamilyName)
{
	sFamilyName_ = sFamilyName;
}

CString VCardIdentificationType::getNickName()
{
	return sNickName_;
}

void VCardIdentificationType::setNickName(CString sNickName)
{
	sNickName_ = sNickName;
}

CString VCardIdentificationType::getHonorificPrefix()
{
	return sHonorificPrefix_;
}

void VCardIdentificationType::setHonorificPrefix(CString sHonorificPrefix)
{
	sHonorificPrefix_ = sHonorificPrefix;
}

CString VCardIdentificationType::getHonorificSuffix()
{
	return sHonorificSuffix_;
}

void VCardIdentificationType::setHonorificSuffix(CString sHonorificSuffix)
{
	sHonorificSuffix_ = sHonorificSuffix;
}

tm* VCardIdentificationType::getBirthDate()
{
	return pBirthDate_;
}

void VCardIdentificationType::setBirthDate(tm* birthDate)
{
	pBirthDate_ = birthDate;
}

CString VCardIdentificationType::getFormattedString()
{
	CString sSEPERATOR = ";";
    CString sFString = "";
	CString sTempString = "";
	
	// Formatted Name
	sFormattedName_.TrimLeft();
	sFormattedName_.TrimRight();
	if(sFormattedName_ != "")
	{
		sTempString = helper_.escapeString(sFormattedName_);
		sFString += "FN:" + sTempString;
	}

	// NAME : "N:
	// Purpose: To specify the components of the name of the object the vCard represents.
    // Family Name, Given Name, Additional Names, Honorific Prefixes, and Honorific Suffixes.
    // The text components are separated by the SEMI-COLON character (ASCII decimal 59). Individual text
    // components can include multiple text values (e.g., multiple Additional Names) separated by the COMMA character (ASCII decimal 44).            
	sFString += "N:";

    // Family name
    sFString += sFamilyName_;

    // Given name
    sFString += ";" + sGivenName_;

    // Additional name
    sFString += ";" + sAdditionalName_;

	//// Honorific Prefix
    //sFString += ";" + sHonorificPrefix_;

    //// Honorific Suffix
    //sFString += ";" + sHonorificSuffix_;


    //// NickName
	//sNickName_.TrimLeft();
	//sNickName_.TrimRight();
    //if (sNickName_.Trim() != "")
    //    sFString += "\r\nNICKNAME:" + sNickName_;

    //// Birth Date
    //if (pBirthDate_ != NULL)
	//{
	//	char cBuffer [80];
	//	strftime (cBuffer, 80,"%F", pBirthDate_);
    //    sFString += "\r\nBDAY:" +  CString(cBuffer, 10); // YYYY-MM-DD
	//}

    return sFString;
}
