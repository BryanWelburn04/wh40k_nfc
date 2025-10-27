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

#include "Stdafx.h"
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
	_instructionClass = 0x00;
	_instructionCode = 0x00;
	_parameter1 = 0x00;
	_parameter2 = 0x00;
	_parameter3 = 0x00;
	_sendLen = 0;
	_receiveLen = 0;
}

Apdu::~Apdu()
{

}


//////////////////////////////////////////////////////////////////////
// Implementation of accessor functions
//////////////////////////////////////////////////////////////////////

void Apdu::setInstructionClass(uint8 instructionClass)
{
	_instructionClass = instructionClass;
}

void Apdu::setInstructionCode(uint8 instructionCode)
{
	_instructionCode = instructionCode;
}

void Apdu::setParameter1(uint8 parameter1)
{
	_parameter1 = parameter1;
}

void Apdu::setParameter2(uint8 parameter2)
{
	_parameter2 = parameter2;
}

void Apdu::setParameter3(uint8 parameter3)
{
	_parameter3 = parameter3;
}

void Apdu::setLengthExpected(uint8 lengthExpected)
{
	_lengthExpected = lengthExpected;
}

void Apdu::setSendData(uint8* sendData)
{
	_sendData = sendData;
}

void Apdu::setReceiveData(uint8* receiveData)
{
	_receiveData = receiveData;
}

void Apdu::setStatusWord(uint8* statusWord)
{
	_statusWord = statusWord;
}

void Apdu::setResponse(uint8* response)
{
	_response = response;
}

void Apdu::setCommand(uint8* command)
{
	/*if ((uint8)sizeof(command) != 5)
		throw "Invalid command";*/

	_instructionClass = command[0];
	_instructionCode = command[1];
	_parameter1 = command[2];
	_parameter2 = command[3];
	_parameter3 = command[4];

}

void Apdu::setSendLen(int sendLen)
{
	_sendLen = sendLen;
}

void Apdu::setReceiveLen(int receiveLen)
{
	_receiveLen = receiveLen;
}

bool Apdu::statusWordEqualTo(uint8* data)
{
	if(_statusWord == NULL)
		return false;

	for(int i=0 ; i < sizeof(_statusWord) ;i++)
		if(_statusWord[i] != data[i])
			return false;

	return true;
}

bool Apdu::statusWordEqualToAccount(uint8* data)
{
	if(_statusWord == NULL)
		return false;

	for(int i=3 ; i < sizeof(_statusWord) ;i++)
		if(_statusWord[i] != data[i])
			return false;

	return true;
}
