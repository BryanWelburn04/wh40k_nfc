//=========================================================================================
//  Author          : Maria Czarina V. Burgos
// 
//  File            : NdefMessage.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Class implementation for handling NDEF Message
// 
//  Date            : April 22, 2014
// 
//  Revision Trail :
//  Date              Author                  Description
// 
//=========================================================================================

#include "stdafx.h"
#include "NdefMessage.h"

//////////////////////////////////////////////////////////////////////
// Constructor/Destructor
//////////////////////////////////////////////////////////////////////
NdefMessage::NdefMessage()
{

}

NdefMessage::~NdefMessage()
{

}

//////////////////////////////////////////////////////////////////////
// Implementation of member functions
//////////////////////////////////////////////////////////////////////
void NdefMessage::appendRecord(NdefRecord cRecord)
{
	ndefRecords_.push_back(cRecord);
}

void NdefMessage::insertRecord(int iIndex, NdefRecord cRecord)
{
	list<NdefRecord>::iterator iter;

	iter = ndefRecords_.begin();

	for(int i = 0; i < iIndex; i++)
		++iter;
	ndefRecords_.insert(iter, cRecord);
}

list<NdefRecord> NdefMessage::getRecords()
{
	 return ndefRecords_;
}

NdefRecord NdefMessage::getRecord(NdefRecordType cRecordType)
{
	for (std::list<NdefRecord>::iterator it = ndefRecords_.begin(); it != ndefRecords_.end(); ++it)
	{
        if ((*it).getRecordType()->getTypeName() == cRecordType.getTypeName() &&
           (*it).getRecordType()->getTypeNameFormat() == cRecordType.getTypeNameFormat())
        {
            return (*it);
        }
	}

    return NULL;
}

int NdefMessage::getNumberOfRecords()
{
	return ndefRecords_.size();
}

vector<byte> NdefMessage::toByteArray()
{
	vector<byte> uTempArray;
	vector<byte> uBuffer;

    for (list<NdefRecord>::iterator iter = ndefRecords_.begin(); iter != ndefRecords_.end(); iter++)
    {
        if (iter == ndefRecords_.begin())
		{
			(*iter).setMessageBegin(true);
		}

        if (iter == ndefRecords_.end())
		{
			(*iter).setMessageEnd(true);
		}

		uTempArray.clear();
		uTempArray = (*iter).encodeToNdef();

		uBuffer.reserve(uBuffer.size() + uTempArray.size());
		uBuffer.insert(uBuffer.end(), uTempArray.begin(), uTempArray.end());
	}
	
	return uBuffer;
}
