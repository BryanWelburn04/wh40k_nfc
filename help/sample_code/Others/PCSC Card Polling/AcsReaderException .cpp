//===========================================================================================
// 
//  Author          : Brey Vaughn Lee
// 
//  File            : AcsReaderException.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Contains Methods for class ProprietaryException
// 
//  Date            : January 7, 2013
// 
//  Revision Traile : [Author] / [Date of modification] / [Details of Modifications done]
// 
// 
//=========================================================================================

#include "stdafx.h"
#include "AcsReaderException.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
AcsReaderException::AcsReaderException()
{
	_message = NULL;
}

AcsReaderException::AcsReaderException(char* errorString)
{
	_message = errorString;
}

AcsReaderException::AcsReaderException(long errorNumber)
{
	
}

AcsReaderException::~AcsReaderException()
{

}

const char* AcsReaderException::what()
{
	return _message;
}
