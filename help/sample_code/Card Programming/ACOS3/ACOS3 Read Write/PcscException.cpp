//===========================================================================================
// 
//  Author          : Teosseth G. Altar
// 
//  File            : PcscException.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Class implementation for handling PC/SC relevant exceptions
// 
//  Date            : October 18, 2011
// 
//  Revision Traile : [Author] / [Date of modification] / [Details of Modifications done]
// 
// 
//=========================================================================================
// PcscException.cpp: implementation of the PcscException class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ACOS Sample Codes.h"
#include "PcscException.h"
#include "PcscProvider.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PcscException::PcscException(char* errorString)
{
	_message = errorString;
}

PcscException::PcscException(long errorNumber)
{
	PcscProvider pcscProvider;
	_message = pcscProvider.getSCardErrorMsg(errorNumber);
}


PcscException::~PcscException()
{

}

const char* PcscException::what()
{
	return _message;
}

