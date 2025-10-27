//===========================================================================================
// 
//  Author          : Valerie C. Dio
// 
//  File            : CardException.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Contains class implementation for card exception.
// 
//  Date            : October 11, 2012
// 
//  Revision Traile : [Author] / [Date of modification] / [Details of Modifications done]
// 
// 
//=========================================================================================

#include "stdafx.h"
#include "Acos3Encrypt.h"
#include "CardException.h"
#include "Acos3.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CardException::CardException()
{
	_message = NULL;	
}

CardException::CardException(char* message, byte* sw)
{
	_message = new char[strlen(message)+1];

	ASSERT(_message != NULL);

	strcpy(_message,message);

	memcpy(_statusWord,sw,2);

	
}

CardException::CardException(char* message)
{
	_message = new char[strlen(message)+1];

	ASSERT(_message != NULL);

	strcpy(_message,message);
	
}

CardException::CardException(const CardException & rhs)
{
	_message = new char[strlen(rhs._message)+1];

	ASSERT(_message != NULL);

	strcpy(_message,rhs._message);

	memcpy(_statusWord,rhs._statusWord,2);
}

CardException::~CardException()
{
	if (_message != NULL)
	{
		delete [] _message;
	}
}

