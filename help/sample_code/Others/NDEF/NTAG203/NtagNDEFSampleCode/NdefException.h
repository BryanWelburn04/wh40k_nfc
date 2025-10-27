/////////////////////////////////////////////////////////////////////////////
// NdefException.h : header file
/////////////////////////////////////////////////////////////////////////////

#pragma once

class NdefException
{
public:	
	NdefException(char* pErrorString); // Constructor
	virtual ~NdefException();          // Destructor
	
	const char* what();

private:
	const char* pMessage_;
};