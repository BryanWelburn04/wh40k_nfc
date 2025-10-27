// PcscException.h: interface for the PcscException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCSCEXCEPTION_H__F1EE3DBD_748F_4B16_8074_C17B9910B64B__INCLUDED_)
#define AFX_PCSCEXCEPTION_H__F1EE3DBD_748F_4B16_8074_C17B9910B64B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PcscException  
{
public:
	const char* what();
	PcscException(long errorNumber);
	PcscException(char* errorString);
	virtual ~PcscException();

private:
	const char* _message;
};

#endif // !defined(AFX_PCSCEXCEPTION_H__F1EE3DBD_748F_4B16_8074_C17B9910B64B__INCLUDED_)
