// PcscException.cpp: implementation of the PcscException class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PcscException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PcscException::PcscException()
{
	iErrorCode_ = 0;
    pMessage_ = NULL;
}

PcscException::PcscException(char* pMessage)
{
	iErrorCode_ = 0;
    pMessage_ = pMessage;
}

PcscException::PcscException(int64 iErrorCode, char* pMessage)
{
    iErrorCode_ = iErrorCode;
    pMessage_ = pMessage;
}

PcscException::~PcscException()
{

}
