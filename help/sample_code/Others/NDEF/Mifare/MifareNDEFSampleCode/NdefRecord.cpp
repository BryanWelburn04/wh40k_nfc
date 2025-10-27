//=========================================================================================
//  Author          : Anne Princess P. Nabus
//					  Maria Czarina V. Burgos
// 
//  File            : NdefRecord.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Class implementation for handling NDEF Records
// 
//  Date            : April 14, 2014
// 
//  Revision Trail :
//  Date              Author                  Description
// 
//=========================================================================================

#include "stdafx.h"
#include "NdefRecord.h"

//////////////////////////////////////////////////////////////////////
// Constructor/Destructor
//////////////////////////////////////////////////////////////////////
NdefRecord::NdefRecord()
{
	
}

NdefRecord::NdefRecord(NdefRecordType* pRecordType)
{
	bIsMessageBegin_ = false;
	bIsMessageEnd_ = false;
	bIsTerminatingRecordChunk_ = true;
	bIsShortRecord_ = false;
	pRecordType_ = pRecordType;
	uPayload_.clear();
	uMessageId_.clear();
}

NdefRecord::~NdefRecord()
{

}

//////////////////////////////////////////////////////////////////////
// Implementation of member functions
//////////////////////////////////////////////////////////////////////
bool NdefRecord::getMessageBegin()
{
	return bIsMessageBegin_;
}

void NdefRecord::setMessageBegin(bool bIsMessageBegin)
{ 
	bIsMessageBegin_ = bIsMessageBegin; 
}

bool NdefRecord::getMessageEnd() 
{ 
	return bIsMessageEnd_; 
}

void NdefRecord::setMessageEnd(bool bIsMessageEnd)
{ 
	bIsMessageEnd_ = bIsMessageEnd;
}

bool NdefRecord::getTerminatingRecordChunk() 
{ 
	return bIsTerminatingRecordChunk_; 
}

void NdefRecord::setTerminatingRecordChunk(bool bIsTerminatingRecordChunk) 
{ 
	bIsTerminatingRecordChunk_ = bIsTerminatingRecordChunk; 
}

bool NdefRecord::getShortRecord()
{
	if(uPayload_.size() < 256)
		bIsShortRecord_ = true;
	else
		bIsShortRecord_ = false;

	return bIsShortRecord_;
}

void NdefRecord::setShortRecord(bool bIsShortRecord)
{
	bIsShortRecord_ = bIsShortRecord;
}

NdefRecordType* NdefRecord::getRecordType()
{ 
	return pRecordType_;
}

void NdefRecord::setRecordType(NdefRecordType* pRecordType)
{ 
	pRecordType_ = pRecordType;
}

vector<byte> NdefRecord::getPayload()
{
	return uPayload_;
}

void NdefRecord::setPayload(vector<byte> uPayload)
{
	uPayload_.clear();
	uPayload_.reserve(uPayload_.size() + uPayload.size());
	uPayload_.insert(uPayload_.end(), uPayload.begin(), uPayload.end());
}

CString NdefRecord::getPayloadString()
{
	return cHelper_.vectorToString(uPayload_);
}

void NdefRecord::setPayloadString(CString sPayload)
{
	uPayload_.clear();
	uPayload_ = cHelper_.stringToVector(sPayload);
}

vector<byte> NdefRecord::getMessageId()
{
	return uMessageId_;
}

void NdefRecord::setMessageId(vector<byte> uMessageId)
{
	uPayload_.clear();
	uMessageId_.reserve(uMessageId_.size() + uMessageId.size());
	uMessageId_.insert(uMessageId_.end(), uMessageId.begin(), uMessageId.end());
}

vector<byte> NdefRecord::encodeToNdef()
{
	byte		 uNdefHeader = 0x00;
    vector<byte> uNdef;
	CString		 sTypeName = "";

	////////////////////////////////////
	//   7    6    5    4    3    2-0
    // | MB | ME | CF | SR | IL | TNF |
    // |<--------- Header ----------->|
	//  ------------------------------
	// |     Payload Type Length      |
	//  ------------------------------
	// |      Payload Length 3        |
	//  ------------------------------
	// |      Payload Length 2        |
	//  ------------------------------
	// |      Payload Length 1        |
	//  ------------------------------
	// |      Payload Length 0        |
	//  ------------------------------
	// |      Message ID Length       |
	//  ------------------------------
	// |        Payload Type          |
	//  ------------------------------
	// |         Message ID           |
	//  ------------------------------
	// |           Payload            |
	//  ------------------------------
    ////////////////////////////////////

    // Set NDEF Header
    if (getMessageBegin())
        uNdefHeader |= 0x80;

    if (getMessageEnd())
        uNdefHeader |= 0x40;

    if (!getTerminatingRecordChunk())
        uNdefHeader |= 0x20;

    if (getShortRecord())
        uNdefHeader |= 0x10;

    if (uMessageId_.size() > 0)
        uNdefHeader |= 0x08;

    uNdefHeader |= (byte)getRecordType()->getTypeNameFormat();

	// Append uNdef to uNdefHeader
	uNdef.push_back(uNdefHeader);

    // Set Payload Type Length
	sTypeName = getRecordType()->getTypeName();
	sTypeName.TrimLeft();
	sTypeName.TrimRight();
    uNdef.push_back((byte)sTypeName.GetLength());

    // Set Payload Length
    if (uPayload_.size() < 256 && getShortRecord())
	{
        uNdef.push_back(uPayload_.size());
	}
    else
	{
		byte* uPayloadLength = cHelper_.intToByte(uPayload_.size());

		for(int i = 0; i < 4; i++)
			uNdef.push_back(uPayloadLength[3-i]);

	}
	
    // Set Message ID Length
    if (uMessageId_.size() > 0)
        uNdef.push_back((byte)uMessageId_.size());

	for(int i = 0; i < sTypeName.GetLength(); i++)
		uNdef.push_back(sTypeName[i]);

    // Set Message ID
    if (uMessageId_.size() > 0)
	{
		for(int j = 0; j < uMessageId_.size(); j++)
			uNdef.push_back(uMessageId_.at(j));
	}

    // Set Payload
	uNdef.reserve(uNdef.size() + uPayload_.size());
	uNdef.insert(uNdef.end(), uPayload_.begin(), uPayload_.end());

	return uNdef;
}

void NdefRecord::appendPayload(vector<byte> uPayload)
{
	uPayload_.reserve(uPayload_.size() + uPayload.size());
	uPayload_.insert(uPayload_.end(), uPayload.begin(), uPayload.end());
}

void NdefRecord::appendPayload(CString sPayload)
{
	for(int i = 0; i < sPayload.GetLength(); i++)
		uPayload_.push_back(sPayload[i]);
}

void NdefRecord::appendPayload(byte uPayload)
{
	uPayload_.push_back(uPayload);
}
