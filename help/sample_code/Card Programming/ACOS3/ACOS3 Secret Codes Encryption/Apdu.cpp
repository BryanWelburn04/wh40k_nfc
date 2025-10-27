//===========================================================================================
// 
//  Author          : Teosseth G. Altar
// 
//  File            : Apdu.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Container class implementation for constructing APDU
// 
//  Date            : October 18, 2011
// 
//  Revision Traile : [Author] / [Date of modification] / [Details of Modifications done]
// 
// 
//=========================================================================================
// Apdu.cpp: implementation of the Apdu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Apdu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Apdu::Apdu()
{
	_instructionClass	= 0x00;
	_instructionCode	= 0x00;
	_parameter1			= 0x00;
	_parameter2			= 0x00;
	_parameter3			= 0x00;
	_sendLen			= 0x00;
	_receiveLen			= 0x00;
}

Apdu::~Apdu()
{

}


//////////////////////////////////////////////////////////////////////
// Implementation of accessor functions
//////////////////////////////////////////////////////////////////////

void Apdu::setInstructionClass(byte instructionClass)
{
	_instructionClass = instructionClass;
}

void Apdu::setInstructionCode(byte instructionCode)
{
	_instructionCode = instructionCode;
}

void Apdu::setParameter1(byte parameter1)
{
	_parameter1 = parameter1;
}

void Apdu::setParameter2(byte parameter2)
{
	_parameter2 = parameter2;
}

void Apdu::setParameter3(byte parameter3)
{
	_parameter3 = parameter3;
}

void Apdu::setLengthExpected(byte lengthExpected)
{
	_lengthExpected = lengthExpected;
}

void Apdu::setSendData(byte* pSendData)
{
	pSendData_ = pSendData;
}

void Apdu::setReceiveData(byte* pReceiveData)
{
	pReceiveData_ = pReceiveData;
}

void Apdu::setStatusWord(byte* pStatusWord)
{
	pStatusWord_ = pStatusWord;
}

void Apdu::setResponse(byte* pResponse)
{
	pResponse_ = pResponse;
}

void Apdu::setCommand(byte* pCommand)
{
	if (pCommand == NULL)
	{
		_instructionClass	= 0;
		_instructionCode	= 0;
		_parameter1			= 0;
		_parameter2			= 0;
		_parameter3			= 0;
	}
	else
	{
		_instructionClass	= pCommand[0];
		_instructionCode	= pCommand[1];
		_parameter1			= pCommand[2];
		_parameter2			= pCommand[3];
		_parameter3			= pCommand[4];
	}	
}

void Apdu::setSendLen(uint32 sendLen)
{
	_sendLen = sendLen;
}

void Apdu::setReceiveLen(uint32 receiveLen)
{
	_receiveLen = receiveLen;
}

bool Apdu::statusWordEqualTo(byte* pData)
{
	if(pStatusWord_ == NULL)
		return false;

	for(int i=0 ; i < 2 ;i++) //todo: magic number
		if(pStatusWord_[i] != pData[i])
			return false;

	return true;
}

bool Apdu::statusWordEqualToAccount(byte* pData)
{
	if(pStatusWord_ == NULL)
		return false;

	for(int i=3 ; i < sizeof(pStatusWord_) ;i++)
		if(pStatusWord_[i] != pData[i])
			return false;

	return true;
}
