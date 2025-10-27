//=========================================================================================
//  Author          : Anne Princess P. Nabus
// 
//  File            : NdefRecordType.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Class implementation for handling NDEF Record Types
// 
//  Date            : April 14, 2014
// 
//  Revision Trail :
//  Date              Author                  Description
// 
//=========================================================================================

#include "stdafx.h"
#include "NdefRecordType.h"
#include "NdefException.h"

//////////////////////////////////////////////////////////////////////
// Constructor/Destructor
//////////////////////////////////////////////////////////////////////
NdefRecordType::NdefRecordType()
{
	
}

NdefRecordType::NdefRecordType(TYPE_NAME_FORMAT eTypeNameFormat, CString sName)
{
	eTypeNameFormat_ = eTypeNameFormat;
	sTypeName_ = sName;
}

NdefRecordType::~NdefRecordType()
{

}

//////////////////////////////////////////////////////////////////////
// Implementation of member functions
//////////////////////////////////////////////////////////////////////
void NdefRecordType::setTypeNameFormat(TYPE_NAME_FORMAT eTypeNameFormat)
{
	eTypeNameFormat_ = eTypeNameFormat;
}

void NdefRecordType::setTypeName(CString sName)
{
	if (strlen(sName) > 255)
		throw NdefException("Payload type name is too long");

	sTypeName_ = sName;
}