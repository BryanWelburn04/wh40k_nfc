#pragma once

#include "Helper.h"

class HelperClass
{

};

class SecurityAttribute
{
public:
	// accessor functions
	bool getIssuerCode() const { return _issuerCode; }
	bool getPin() const { return _pin; }
	bool getAccessCondition5() const { return _accessCondition5; }
	bool getAccessCondition4() const { return _accessCondition4; }
	bool getAccessCondition3() const { return _accessCondition3; }
	bool getAccessCondition2() const { return _accessCondition2; }
	bool getAccessCondition1() const { return _accessCondition1; }

	// mutator functions
	void setIssuerCode(bool issuerCode);
	void setPin(bool pin);
	void setAccessCondition5(bool accessCondition5);
	void setAccessCondition4(bool accessCondition4);
	void setAccessCondition3(bool accessCondition3);
	void setAccessCondition2(bool accessCondition2);
	void setAccessCondition1(bool accessCondition1);

	void securityAttribute(byte securityAttribute);
	byte getRawValue();


private:
	bool _issuerCode;
    bool _pin;
    bool _accessCondition5;
    bool _accessCondition4;
    bool _accessCondition3;
    bool _accessCondition2;
    bool _accessCondition1;
};

typedef byte INQUIRE_RESPONSE[25];

class InquireAccountResponse
{
	public:
	// accessor functions
	byte* getMac() const { return _mac; }
	byte getTransactionType() const { return _transactionType; }
	int getBalance() const 
	{ 
		Helper* helper = new Helper();
		return helper->byteToInt(_balance);
	}
	byte* getAtref() const { return _atref; }
	int getMaximumBalanceAllowed() const 
	{ 
		Helper* helper = new Helper();
		return helper->byteToInt(_maximumBalanceAllowed); 
	}
	byte* getTtrefc() const { return _ttrefc; }
	byte* getTtrefd() const { return _ttrefd; }

	// mutator functions
	void setMac(byte* mac);
	void setTransactionType(byte transactionType);
	void setBalance(int balance);
	int setBalanceInt();
	void setAtref(byte* atref);
	void setMaximumBalanceAllowed(int maximumBalanceAllowed);
	void setTtrefc(byte* ttrefc);
	void setTtrefd(byte* ttrefd);

	byte* inquireAccountResponse(byte* rawDataFromCard);
	byte* inquireAccountResponse(byte* rawDataFromCard, int recvLen);

private:
	byte* _mac;
    byte _transactionType;
    byte* _balance;
    byte* _atref;
    byte* _maximumBalanceAllowed;
    byte* _ttrefc;
	byte* _ttrefd;
	byte* rawDataFromCard;
};

class OptionRegister
{
public:
	// accessor functions
	bool getEnableAccount() const {return _enableAccount;}
	bool getEnableTripleDes() const {return _enableTripleDes;}
	bool getEnableChangePinCommand() const {return _enableChangePinCommand;}
	bool getEnableDebitMac() const {return _enableDebitMac;}
	bool getRequirePinDuringDebit() const {return _requirePinDuringDebit;}
	bool getEnableRevokeDebitCommand() const {return _enableRevokeDebitCommand;}
	bool getRequireMutualAuthenticationOnAccountTransaction() const {return _requireMutualAuthenticationOnAccountTransaction;}
	bool getRequireMutualAuthenticationOnInquireAccount() const {return _requireMutualAuthenticationOnInquireAccount;}

	// mutator functions
	void setEnableAccount(bool enableAccount);
	void setEnableTripleDes(bool enableTripleDes);
	void setEnableChangePinCommand(bool enableChangePinCommand);
	void setEnableDebitMac(bool enableDebitMac);
	void setRequirePinDuringDebit(bool requirePinDuringDebit);
	void setEnableRevokeDebitCommand(bool enableRevokeDebitCommand);
	void setRequireMutualAuthenticationOnAccountTransaction(bool requireMutualAuthenticationOnAccountTransaction);
	void setRequireMutualAuthenticationOnInquireAccount(bool requireMutualAuthenticationOnInquireAccount);

	void optionRegister(byte optioRegisterByte);
	byte getRawValue();

private:
	bool _enableAccount;
	bool _enableTripleDes;
	bool _enableChangePinCommand;
	bool _enableDebitMac;
	bool _requirePinDuringDebit;
	bool _enableRevokeDebitCommand;
	bool _requireMutualAuthenticationOnAccountTransaction;
	bool _requireMutualAuthenticationOnInquireAccount;
};

class SecurityOptionRegister
{
public:
	// accessor functions
	bool getIssuerCode() const { return _issuerCode; }
	bool getPin() const { return _pin; }
	bool getAccessCondition5() const { return _accessCondition5; }
	bool getAccessCondition4() const { return _accessCondition4; }
	bool getAccessCondition3() const { return _accessCondition3; }
	bool getAccessCondition2() const { return _accessCondition2; }
	bool getAccessCondition1() const { return _accessCondition1; }

	// mutator functions
	void setIssuerCode(bool issuerCode);
	void setPin(bool pin);
	void setAccessCondition5(bool accessCondition5);
	void setAccessCondition4(bool accessCondition4);
	void setAccessCondition3(bool accessCondition3);
	void setAccessCondition2(bool accessCondition2);
	void setAccessCondition1(bool accessCondition1);

	void securityAttribute(byte securityAttribute);
	byte getRawValue();


private:
	bool _issuerCode;
    bool _pin;
    bool _accessCondition5;
    bool _accessCondition4;
    bool _accessCondition3;
    bool _accessCondition2;
    bool _accessCondition1;
};