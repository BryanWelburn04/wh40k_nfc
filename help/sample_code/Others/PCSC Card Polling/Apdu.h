// Apdu.h: interface for the Apdu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APDU_H__96969710_0CED_4065_BD24_67EFD141F7C8__INCLUDED_)
#define AFX_APDU_H__96969710_0CED_4065_BD24_67EFD141F7C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ACSCommon.h"

class Apdu  
{
public:

	// accessor functions
	int getReceiveLen() const { return _receiveLen; }
	int getSendLen() const { return _sendLen; } 
	uint8* getStatusWord() const { return _statusWord; }
	uint8* getReceiveData() const { return _receiveData; }
	uint8* getSendData() const { return _sendData; }
	uint8* getResponse() const { return _response; }
	uint8 getLengthExpected() const { return _lengthExpected; }
	uint8 getParameter3() const { return _parameter3; }
	uint8 getParameter2() const { return _parameter2; }
	uint8 getParameter1() const { return _parameter1; }
	uint8 getInstructionCode() const { return _instructionCode; }
	uint8 getInstructionClass() const { return _instructionClass; }

	// mutator functions
	void setReceiveLen(int receiveLen);
	void setSendLen(int sendLen);
	void setCommand(uint8* command);
	void setStatusWord(uint8* statusWord);
	void setReceiveData(uint8* receiveData);
	void setSendData(uint8* sendData);
	void setResponse(uint8* response);
	void setLengthExpected(uint8 lengthExpected);
	void setParameter3(uint8 bP3);
	void setParameter2(uint8 bP2);
	void setParameter1(uint8 bP1);
	void setInstructionCode(uint8 bIns);
	void setInstructionClass(uint8 bCla);
	bool statusWordEqualTo(uint8* data);
	bool statusWordEqualToAccount(uint8* data);
		
	Apdu();
	virtual ~Apdu();

private:

	int _receiveLen;
	int _sendLen;
	uint8* _statusWord;
	uint8* _receiveData;
	uint8* _sendData;
	uint8* _response;
	uint8 _lengthExpected;
	uint8 _parameter3;
	uint8 _parameter2;
	uint8 _parameter1;
	uint8 _instructionCode;
	uint8 _instructionClass;

};

#endif // !defined(AFX_APDU_H__96969710_0CED_4065_BD24_67EFD141F7C8__INCLUDED_)
