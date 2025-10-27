// Apdu.h: interface for the Apdu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APDU_H__96969710_0CED_4065_BD24_67EFD141F7C8__INCLUDED_)
#define AFX_APDU_H__96969710_0CED_4065_BD24_67EFD141F7C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Apdu  
{
public:

	// accessor functions
	int getReceiveLen() const { return _receiveLen; }
	int getSendLen() const { return _sendLen; } 
	byte* getStatusWord() const { return _statusWord; }
	byte* getReceiveData() const { return _receiveData; }
	byte* getSendData() const { return _sendData; }
	byte* getResponse() const { return _response; }
	byte getLengthExpected() const { return _lengthExpected; }
	byte getParameter3() const { return _parameter3; }
	byte getParameter2() const { return _parameter2; }
	byte getParameter1() const { return _parameter1; }
	byte getInstructionCode() const { return _instructionCode; }
	byte getInstructionClass() const { return _instructionClass; }

	// mutator functions
	void setReceiveLen(int receiveLen);
	void setSendLen(int sendLen);
	void setCommand(byte* command);
	void setStatusWord(byte* statusWord);
	void setReceiveData(byte* receiveData);
	void setSendData(byte* sendData);
	void setResponse(byte* response);
	void setLengthExpected(byte lengthExpected);
	void setParameter3(byte bP3);
	void setParameter2(byte bP2);
	void setParameter1(byte bP1);
	void setInstructionCode(byte bIns);
	void setInstructionClass(byte bCla);
	bool statusWordEqualTo(byte* data);
	bool statusWordEqualToAccount(byte* data);
		
	Apdu();
	virtual ~Apdu();

private:

	int _receiveLen;
	int _sendLen;
	byte* _statusWord;
	byte* _receiveData;
	byte* _sendData;
	byte* _response;
	byte _lengthExpected;
	byte _parameter3;
	byte _parameter2;
	byte _parameter1;
	byte _instructionCode;
	byte _instructionClass;

};

#endif // !defined(AFX_APDU_H__96969710_0CED_4065_BD24_67EFD141F7C8__INCLUDED_)
