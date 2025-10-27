// PcscException.h: interface for the PcscException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCSCEXCEPTION_H__BD15D08F_ED5D_4978_AC52_40380545E4A5__INCLUDED_)
#define AFX_PCSCEXCEPTION_H__BD15D08F_ED5D_4978_AC52_40380545E4A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AcsCommon.h"

class PcscException
{
public:
	PcscException();
	PcscException(char *pMessage);
    PcscException(int64 iErrorCode, char *pMessage);

    long getErrorCode() const { return iErrorCode_; }
    char* getMessage() const { return pMessage_; }

    virtual ~PcscException();

private:
    char *pMessage_;
    long iErrorCode_;
};

#endif // !defined(AFX_PCSCEXCEPTION_H__BD15D08F_ED5D_4978_AC52_40380545E4A5__INCLUDED_)
