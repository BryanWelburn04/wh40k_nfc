//===========================================================================================
//  Author          : Anne Princess P. Nabus
// 
//  File            : NdefException.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Class implementation for handling NDEF relevant exceptions
// 
//  Date            : April 14, 2014
// 
//  Revision Trail : [Author] / [Date of modification] / [Details of Modifications done]
// 
//=========================================================================================

#include "stdafx.h"
#include "NdefException.h"

//////////////////////////////////////////////////////////////////////
// Constructor/Destructor
//////////////////////////////////////////////////////////////////////
NdefException::NdefException(char* pErrorString)
{
	pMessage_ = pErrorString;
}

NdefException::~NdefException()
{

}

//////////////////////////////////////////////////////////////////////
// Implementation of member functions
//////////////////////////////////////////////////////////////////////
const char* NdefException::what()
{
	return pMessage_;
}

