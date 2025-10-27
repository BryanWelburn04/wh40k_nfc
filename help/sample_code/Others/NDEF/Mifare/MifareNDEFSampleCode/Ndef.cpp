//=========================================================================================
//  Author          : Maria Czarina V. Burgos
// 
//  File            : Ndef.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Class implementation for handling NDEF
// 
//  Date            : April 22, 2014
// 
//  Revision Trail :
//  Date              Author                  Description
// 
//=========================================================================================

#include "stdafx.h"
#include "Ndef.h"

//////////////////////////////////////////////////////////////////////
// Constructor/Destructor
//////////////////////////////////////////////////////////////////////
Ndef::Ndef()
{

}

Ndef::~Ndef()
{

}

//////////////////////////////////////////////////////////////////////
// Implementation of member functions
//////////////////////////////////////////////////////////////////////
NdefMessage* Ndef::getNestedNdefMessage(NdefRecord* pNdefRecord)
{
	if (pNdefRecord->getPayload().size() == 0)
        throw new NdefException("Payload is not yet been set");

    return getNestedNdefMessage(0, pNdefRecord->getPayload());
}

NdefMessage* Ndef::getNestedNdefMessage(int iIndex, vector<byte> uData)
{
	NdefMessage* pNdefMessage = new NdefMessage();
    NdefRecord*  pNdefRecord;
    byte uTypeNameFormat = 0x00;
    byte uTypeNameLength = 0x00;
    byte uIdLength = 0x00;

    int iPayloadLength = 0;
    int iCurrentIndex = 0;
	int iIdx = 0;

    CString sTypeName = "";

    bool bIsMessageBeginSet = false;
    bool bIsMessageEndSet = false;
    bool bIsIdLengthPresent = false;
    bool bIsChunkFlagPresent = false;
    bool bIsShortRecordSet = false;

	vector<byte> uDataTemp;
	vector<byte> uDataTemp2;
	vector<byte> uDataTemp3;
	vector<byte> uDataTemp4;

	char cMessage[200];

    if (uData.size() <= iIndex)
        throw new NdefException("Invalid index");

    while(iIndex < uData.size())
    {
		iCurrentIndex = iIndex;

        // Get Type Name Format
        // e.g. NFC Well Know Type = 0x01
        uTypeNameFormat = (byte)(uData.at(iCurrentIndex) & 0x07);

        if (uTypeNameFormat != 0x01 && uTypeNameFormat != 0x02)
		{
			strcpy(cMessage, "Type Name Format "); // Copy string one into the result.
			strcat(cMessage, cHelper_.byteAsString((TYPE_NAME_FORMAT)(uTypeNameFormat)));
			strcat(cMessage, " is not supported.");
			throw new NdefException(cMessage);
		}
        
        // Check if Message Begin (Bit 7) is set 
        if ((uData[iCurrentIndex] & 0x80) != 0x00)
            bIsMessageBeginSet = true;
        else
            bIsMessageBeginSet = false;

        // Check if Message End (Bit 6) is set
        if ((uData[iCurrentIndex] & 0x40) != 0x00)
            bIsMessageEndSet = true;
        else
            bIsMessageEndSet = false;

        // Check if Chunk Flag (Bit 5) is set
        if ((uData[iCurrentIndex] & 0x20) != 0x00)
            bIsChunkFlagPresent = true;
        else
            bIsChunkFlagPresent = false;

        // Check if Short Record (bit 4) is set
        if ((uData[iCurrentIndex] & 0x10) != 0x00)
            bIsShortRecordSet = true;
        else
            bIsShortRecordSet = false;

        // Check if ID length is set
        if ((uData[iCurrentIndex] & 0x08) != 0x00)
            bIsIdLengthPresent = true;
        else
            bIsIdLengthPresent = false;

        iCurrentIndex += 1;

        // Get the type length
        // Refer for Short Record section of NFC Data Exchange Format
        // Technical Specification for more information
        uTypeNameLength = uData[iCurrentIndex];
        iCurrentIndex += 1;

        if (bIsShortRecordSet)
        {
            // For Short Record payload, length is 1 byte
            // Get Payload Length    
            iPayloadLength = uData.at(iCurrentIndex);
            iCurrentIndex += 1;
        }
        else
        {
            // For Non Short Record payload, length is 4 bytes
			uDataTemp.clear();
			for (iIdx = 0; iIdx < 4; iIdx++) 
				uDataTemp.push_back(uData.at(iCurrentIndex + iIdx));
            iPayloadLength = cHelper_.vectorToInt(uDataTemp);
            iCurrentIndex += 4;
        }

        if (bIsIdLengthPresent)
        {
            // +1 to get ID Length
            uIdLength = uData.at(iCurrentIndex);
            iCurrentIndex += 1;
        }

        // +1 to get Payload Type Name
        // Payload Type Name offset = currentIndex to currentIndex  + Type Name Length 
		uDataTemp2.clear();
		for (iIdx = 0; iIdx < (int)uTypeNameLength; iIdx++) 
			uDataTemp2.push_back(uData.at(iCurrentIndex + iIdx));

		sTypeName = cHelper_.vectorToAscii(uDataTemp2);
        iCurrentIndex += uTypeNameLength;
        
        // Initialize new ndef record object
        pNdefRecord = new NdefRecord(new NdefRecordType((TYPE_NAME_FORMAT)(uTypeNameFormat), sTypeName));
        pNdefRecord->setMessageBegin(bIsMessageBeginSet);
        pNdefRecord->setMessageEnd(bIsMessageEndSet);
        pNdefRecord->setTerminatingRecordChunk(!bIsChunkFlagPresent);

        // If ID Length is present get record ID
		uDataTemp3.clear();
        if (bIsIdLengthPresent)
        {
			for (iIdx = 0; iIdx < (int)uIdLength; iIdx++) 
				uDataTemp3.push_back(uData.at(iCurrentIndex + iIdx));
            pNdefRecord->setMessageId(uDataTemp3);
            iCurrentIndex += uIdLength;
        }

		uDataTemp4.clear();

		for (iIdx = 0; iIdx < iPayloadLength && (iCurrentIndex + iIdx) < uData.size(); iIdx++)
			uDataTemp4.push_back(uData.at(iCurrentIndex + iIdx));

        pNdefRecord->setPayload(uDataTemp4);
        iCurrentIndex += iPayloadLength;
        iIndex = iCurrentIndex;

        pNdefMessage->appendRecord(*(pNdefRecord));
    }

	delete pNdefRecord;

    return pNdefMessage;
}

CString Ndef::decodeNdefMessage(vector<byte> uData)
{
	int i = 0;
	int j = 0;
	int	iIndex = 0;
	int	iRecordLen = 0;
	int	iPayloadLen = 0;
	int	iPayloadSize = 0;
	int iStatusByteLen = uData[iIndex] & 0x3F;
	int	iCurrIdx = 0;
	int	iEndIdx	= 0;
	CString	sSeparator = ";";
	CString sDisplayTextTemp = "";
	
	vector<int>  iPayloadLength;
	vector<byte> uDisplayText;

	byte	uTypeNameFormat = 0x00;

	CString	sDisplayText = "";
    CString	sRecordName  = "";

	bool	bIsMessageBeginSet	= false;
    bool	bIsMessageEndSet		= false;
	bool	bIsIdLengthPresent	= false;
    bool	bIsChunkFlagPresent	= false;
    bool	bIsShortRecordSet	= false;

	iPayloadSize = uData.at(2);

	while(iIndex < iPayloadSize)
    {
        // Get Type Name Format
        // e.g. NFC Well Know Type = 0x01
        uTypeNameFormat = (byte)(uData.at(iIndex) & 0x07);

        if (uTypeNameFormat != 0x01 && uTypeNameFormat != 0x02)
		{
			throw NdefException("Type Name format is not supported.");
		}
        
        // Check if Message Begin (Bit 7) is set 
        if ((uData[iIndex] & 0x80) != 0x00)
            bIsMessageBeginSet = true;
        else
            bIsMessageBeginSet = false;

        // Check if Message End (Bit 6) is set
        if ((uData[iIndex] & 0x40) != 0x00)
            bIsMessageEndSet = true;
        else
            bIsMessageEndSet = false;

        // Check if Chunk Flag (Bit 5) is set
        if ((uData[iIndex] & 0x20) != 0x00)
            bIsChunkFlagPresent = true;
        else
            bIsChunkFlagPresent = false;

        // Check if Short Record (bit 4) is set
        if ((uData[iIndex] & 0x10) != 0x00)
            bIsShortRecordSet = true;
        else
            bIsShortRecordSet = false;

		iIndex++;

		iRecordLen = uData[iIndex];
		iIndex++;

		if(bIsShortRecordSet)
			iPayloadLen = 1;
		else
			iPayloadLen = 4;

		iPayloadLength.clear();
		for(i = 0; i < iPayloadLen; i++)
		{
			iPayloadLength.push_back(uData[iIndex]);
			iIndex++;
		}

		sDisplayText += "Record Name: ";
		sRecordName = "";

		for (i = 0; i < iRecordLen; i++)
		{
			sRecordName += (char)uData[iIndex];
			sDisplayText += (char)uData[iIndex];
			iIndex++;
		}

		sDisplayText += "\r\n";
	
		if(sRecordName == "Sp")
		{

		}
		else if(sRecordName == "T")
		{
			for(j = 0; j < iPayloadLen; j++)
			{
				iIndex++;
        
				sDisplayText += "ISO Language Code: ";
        
				for (i = 0; i < iStatusByteLen; i++)
				{
            		sDisplayText += (char)uData[iIndex];
					iIndex++;
				}
        
				sDisplayText += "\r\nText: ";
				uDisplayText.clear();
				uDisplayText.reserve(iPayloadLength.at(j) - iStatusByteLen - 1);
				uDisplayText.insert(uDisplayText.end(), uData.begin() + iIndex, uData.begin() + iIndex + (iPayloadLength.at(j) - iStatusByteLen - 1));
				sDisplayText += cHelper_.vectorToAscii(uDisplayText);
				sDisplayText += "\r\n";
				
				iIndex += (iPayloadLength.at(j) - iStatusByteLen - 1);
			}
		}
		else if(sRecordName == "U")
		{
			for(j = 0; j < iPayloadLen; j++)
			{
				sDisplayText += "Abbreviation: ";
				sDisplayText += getURIIdentifierCode((URI_IDENTIFIER_CODE)uData[iIndex]);
				iIndex++;
        
				sDisplayText += "\r\nURI: ";
				uDisplayText.clear();
				uDisplayText.reserve(iPayloadLength.at(j) - 1);
				uDisplayText.insert(uDisplayText.end(), uData.begin() + iIndex, uData.begin() + iIndex + (iPayloadLength.at(j) - 1));
				sDisplayText += cHelper_.vectorToAscii(uDisplayText);
				sDisplayText += "\r\n";

				iIndex += (iPayloadLength.at(j) - 1);
			}
		}
		else if(sRecordName == "act")
		{
			sDisplayText += "Action: ";
			sDisplayText += getActionCode((ACTION_RECORD)uData[iIndex]);
			sDisplayText += "\r\n";

			for(j = 0; j < iPayloadLen; j++)
			{
				iIndex += iPayloadLength.at(j);
			}
		}
		else if(sRecordName == "text/x-vCard")
		{
			sDisplayText += "vCard: ";
			for(j = 0; j < iPayloadLen; j++)
			{
				for(i = 0; i < iPayloadLen; i++)
				{
					uDisplayText.clear();
					uDisplayText.reserve(iPayloadLength.at(j) - 1);
					uDisplayText.insert(uDisplayText.end(), uData.begin() + iIndex, uData.begin() + iIndex + (iPayloadLength.at(j) - 1));
					
					sDisplayTextTemp += cHelper_.vectorToAscii(uDisplayText);
					
					iCurrIdx  = sDisplayTextTemp.Find("\r\nN:", 0);
					iEndIdx   = sDisplayTextTemp.Find("END:VCARD", 0);
					
					for(iCurrIdx += 4; iCurrIdx < iEndIdx; iCurrIdx++)
					{
						if(sDisplayTextTemp[iCurrIdx] == sSeparator)
							sDisplayText += " ";
						else
							sDisplayText += sDisplayTextTemp[iCurrIdx];
					}
				}

				iIndex += iPayloadLength.at(j);
			}
		}
	}

	return sDisplayText;
}

vector<byte> Ndef::encodeSmartPoster(CString sTitleLanguage, CString sTitle, URI_IDENTIFIER_CODE eUriPrefix, CString sUri, ACTION_RECORD action)
{
	/// <summary>
    /// Encode the following paramter for NDEF SmartPoster
    /// </summary>
    /// <param name="titleLanguage"></param>
    /// <param name="title"></param>
    /// <param name="uriPrefix"></param>
    /// <param name="uri"></param>
    /// <param name="action"></param>
    /// <returns></returns>
    NdefRecord*		pNdefRecord;
    NdefMessage*	pNdefMessage = new NdefMessage();
	vector<byte>	uBuffer;
    bool			bIsUseUTF8 = true;
    byte			uStatusByte = 0x00;

	NdefRecordType* pNdefRecordType1 = new NdefRecordType(NFC_FORM_WELL_KNOWN_TYPE, "T");
	NdefRecordType* pNdefRecordType2 = new NdefRecordType(NFC_FORM_WELL_KNOWN_TYPE, "act");
	NdefRecordType* pNdefRecordType3 = new NdefRecordType(NFC_FORM_WELL_KNOWN_TYPE, "U");
	NdefRecordType* pNdefRecordType4 = new NdefRecordType(NFC_FORM_WELL_KNOWN_TYPE, "Sp");

    // Encode Title
	sTitle.TrimLeft();
	sTitle.TrimRight();

    // If UTF 8  set bit 7 to 0
    // If UTF 16 set bit 7 to 1
    if(!bIsUseUTF8)
        uStatusByte = (byte) 0x80;
       
    // Length of the Language (ISO/IANA language code)
    // ex. "en-US"
    uStatusByte |= ((byte)sTitleLanguage.GetLength() & 0x3F);

    pNdefRecord = new NdefRecord(pNdefRecordType1);
    pNdefRecord->setMessageBegin(true);
    pNdefRecord->appendPayload(uStatusByte);
    pNdefRecord->appendPayload(sTitleLanguage);
    pNdefRecord->appendPayload(sTitle);

    pNdefMessage->appendRecord(*(pNdefRecord));

	delete pNdefRecord;

	////////////////////////////////////////////////////////////////////////
    // Encode Action
	////////////////////////////////////////////////////////////////////////
    if (action != NO_ACTION)
    {
        pNdefRecord = new NdefRecord(pNdefRecordType2);
        pNdefRecord->appendPayload((byte)action);

        pNdefMessage->appendRecord(*(pNdefRecord));

		delete pNdefRecord;
    }

	////////////////////////////////////////////////////////////////////////
    // Encode URI
	////////////////////////////////////////////////////////////////////////
	if (sUri == "")
		throw NdefException("URI is empty.");

	// URI Prefix
    // See URI Record Type Definition 
    // http://www.nfc-forum.org
    pNdefRecord = new NdefRecord(pNdefRecordType3);
	pNdefRecord->setMessageEnd(true);
    pNdefRecord->appendPayload((byte)eUriPrefix);
    pNdefRecord->appendPayload(sUri);

    pNdefMessage->appendRecord(*(pNdefRecord));

	delete pNdefRecord;

	////////////////////////////////////////////////////////////////////////
    // Smart Poster Header
	////////////////////////////////////////////////////////////////////////
    pNdefRecord = new NdefRecord(pNdefRecordType4);
    pNdefRecord->setMessageBegin(true);
    pNdefRecord->setMessageEnd(true);
    pNdefRecord->appendPayload(pNdefMessage->toByteArray());

    // Encode NDEF Message
    uBuffer = pNdefRecord->encodeToNdef();

	//delete pNdefRecordType;
	delete pNdefRecord;

	delete pNdefRecordType1;
	delete pNdefRecordType2;
	delete pNdefRecordType3;
	delete pNdefRecordType4;
	delete pNdefMessage;
    return uBuffer;
}

vector<byte> Ndef::encodeNfcForumExternalType(CString sType, CString sPayload)
{
	NdefRecordType*	pNdefRecordType = new NdefRecordType(NFC_FORM_EXTERNAL_TYPE, sType);
	NdefRecord*		pNdefRecord = new NdefRecord(pNdefRecordType);
    NdefMessage*	pNdefMessage = new NdefMessage();
	vector<byte>	uBuffer;

    pNdefRecord->setMessageBegin(true);
    pNdefRecord->setMessageEnd(true);
    pNdefRecord->appendPayload(sPayload);

    // Encode NDEF Message
    uBuffer = pNdefRecord->encodeToNdef();
	
	delete pNdefRecordType;	
	delete pNdefRecord;
	delete pNdefMessage;
    return uBuffer;
}

CString Ndef::getURIIdentifierCode(URI_IDENTIFIER_CODE code)
{
	// For more information please refer to
    // URI Record Type Definition Technical Specification
    switch (code)
    {
        case HTTP: return "http://";
        case HTTPS: return "https://";
        case HTTP_WWW: return "http://www.";
        case HTTPS_WWW: return "https://www.";
        case TEL: return "tel:";
        case MAIL_TO: return "mailto:";
        case FTP: return "ftp://";
        case NONE: return "";
        default: return "Unknown";
    }
}

CString Ndef::getActionCode(ACTION_RECORD code)
{
    switch (code)
    {
		case DO_THE_ACTION: return "Do the action";
		case SAVE_FOR_LATER: return "Save for later";
		case OPEN_FOR_EDITING: return "Open for editing";
		default: return "";
    }
}

bool Ndef::isTelUriValid(CString sUri)
{
	for (int i = 0; i < sUri.GetLength(); i++)
	{
		if (!isdigit(sUri[i]) && sUri[i] != '-' && 
			sUri[i] != '+' && sUri[i] != '(' && sUri[i] != ')')
		{
			return false;
		}
	}

	return true;
}

bool Ndef::isNameValid(CString sName)
{
	for (int i = 0; i < sName.GetLength(); i++)
	{
		if (!isalpha(sName[i]) && !isspace(sName[i]) &&
			sName[i] != '-' && sName[i] != '.')
		{
			return false;
		}
	}

	return true;
}
