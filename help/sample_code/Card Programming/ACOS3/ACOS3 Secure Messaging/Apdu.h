// Apdu.h: interface for the Apdu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APDU_H__96969710_0CED_4065_BD24_67EFD141F7C8__INCLUDED_)
#define AFX_APDU_H__96969710_0CED_4065_BD24_67EFD141F7C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AcsCommon.h"

class Apdu  
{
public:

	// accessor functions
	uint32 getReceiveLen() const { return _receiveLen; }
	uint32 getSendLen() const { return _sendLen; } 
	byte* getStatusWord() const { return pStatusWord_; }
	byte* getReceiveData() const { return pReceiveData_; }
	byte* getSendData() const { return pSendData_; }
	byte* getResponse() const { return pResponse_; }
	byte getLengthExpected() const { return _lengthExpected; }
	byte getParameter3() const { return _parameter3; }
	byte getParameter2() const { return _parameter2; }
	byte getParameter1() const { return _parameter1; }
	byte getInstructionCode() const { return _instructionCode; }
	byte getInstructionClass() const { return _instructionClass; }

	// mutator functions
	void setReceiveLen(uint32 receiveLen);
	void setSendLen(uint32 sendLen);
	void setCommand(byte* command);
	void setStatusWord(byte* statusWord);
	void setReceiveData(byte* receiveData);
	void setSendData(byte* sendData);
	void setResponse(byte* response);
	void setLengthExpected(byte lengthExpected);
	void setParameter1(byte uP1);
	void setParameter2(byte uP2);
	void setParameter3(byte uP3);	
	void setInstructionCode(byte bIns);
	void setInstructionClass(byte bCla);
	bool statusWordEqualTo(byte* pData);
	bool statusWordEqualToAccount(byte* pData);
		
	Apdu();
	virtual ~Apdu();

private:

	uint32 _receiveLen;
	uint32 _sendLen;
	byte* pStatusWord_;
	byte* pReceiveData_;
	byte* pSendData_;
	byte* pResponse_;
	byte _lengthExpected;
	byte _parameter3;
	byte _parameter2;
	byte _parameter1;
	byte _instructionCode;
	byte _instructionClass;

};

#endif // !defined(AFX_APDU_H__96969710_0CED_4065_BD24_67EFD141F7C8__INCLUDED_)
