#include "stdafx.h"
//===========================================================================================
// 
//  Author          : Brey Vaughn Lee
// 
//  File            : ProprietaryException.cpp
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


#include "PcscException.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
PcscException::PcscException()
{
	_message = NULL;
}

PcscException::PcscException(char* errorString)
{
	_message = errorString;
}

PcscException::PcscException(long errorNumber)
{
	
}

PcscException::~PcscException()
{

}

const char* PcscException::what()
{
	return _message;
}


