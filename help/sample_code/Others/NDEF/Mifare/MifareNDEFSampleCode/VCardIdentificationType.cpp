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
	CString sSeperator = ";";
    CString sFString = "";
	CString sTempString = "";
	
	// Formatted Name
	sFormattedName_.TrimLeft();
	sFormattedName_.TrimRight();
	if(sFormattedName_ != "")
	{
		sTempString = cHelper_.escapeString(sFormattedName_);
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

    return sFString;
}

void VCardIdentificationType::loadData(vector<byte> uData)
{
	loadData(cHelper_.vectorToAscii(uData));
}

void VCardIdentificationType::loadData(CString sDataAsString)
{
	CString sVProperties;
	CString sTempString = "";
	CString sTemp = "";
	vector<CString> sVPropertiesList;

	int i = 0; // Substring index to extract
	while (AfxExtractSubString(sVProperties, sDataAsString, i, (char)'\r\n'))
	{   
		if(sVProperties != 10) // Line Feed
			sVPropertiesList.push_back(sVProperties);
		i++;

	}

	for(int j = 0; j < sVPropertiesList.size(); j++)
	{
		if(sVPropertiesList.at(i) == "")
			continue;

		sTempString = sVPropertiesList.at(i);
		sTempString.TrimLeft();
		sTempString.TrimRight();
		sTempString.Replace("\n", "");
		sTempString.Replace("\r", "");
		sTempString.Replace("\0", "");

		// Check if formatted Name
		sTemp = sVPropertiesList.at(i).Mid(0,2);
		sTemp.MakeLower();
        if(sTemp == "fn")
        {
            sFormattedName_ = sVPropertiesList.at(3);
            continue;
        }

		// Check if Name
		sTemp = sVPropertiesList.at(i).Mid(0, 2);
		sTemp.MakeLower();
        if (sTemp == "n")
        {
            sFormattedName_ = sVPropertiesList.at(2);
            continue;
        }
	}
}