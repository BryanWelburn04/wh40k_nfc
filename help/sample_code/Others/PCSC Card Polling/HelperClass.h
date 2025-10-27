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

	void securityAttribute(uint8 securityAttribute);
	uint8 getRawValue();


private:
	bool _issuerCode;
    bool _pin;
    bool _accessCondition5;
    bool _accessCondition4;
    bool _accessCondition3;
    bool _accessCondition2;
    bool _accessCondition1;
};

typedef uint8 INQUIRE_RESPONSE[25];

class InquireAccountResponse
{
	public:
	// accessor functions
	uint8* getMac() const { return _mac; }
	uint8 getTransactionType() const { return _transactionType; }
	int getBalance() const 
	{ 
		Helper* helper = new Helper();
		return helper->byteToInt(_balance);
	}
	uint8* getAtref() const { return _atref; }
	int getMaximumBalanceAllowed() const 
	{ 
		Helper* helper = new Helper();
		return helper->byteToInt(_maximumBalanceAllowed); 
	}
	uint8* getTtrefc() const { return _ttrefc; }
	uint8* getTtrefd() const { return _ttrefd; }

	// mutator functions
	void setMac(uint8* mac);
	void setTransactionType(uint8 transactionType);
	void setBalance(int balance);
	int setBalanceInt();
	void setAtref(uint8* atref);
	void setMaximumBalanceAllowed(int maximumBalanceAllowed);
	void setTtrefc(uint8* ttrefc);
	void setTtrefd(uint8* ttrefd);

	uint8* inquireAccountResponse(uint8* rawDataFromCard);
	uint8* inquireAccountResponse(uint8* rawDataFromCard, int recvLen);

private:
	uint8* _mac;
    uint8 _transactionType;
    uint8* _balance;
    uint8* _atref;
    uint8* _maximumBalanceAllowed;
    uint8* _ttrefc;
	uint8* _ttrefd;
	uint8* rawDataFromCard;
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

	void optionRegister(uint8 optioRegisterByte);
	uint8 getRawValue();

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

	void securityAttribute(uint8 securityAttribute);
	uint8 getRawValue();


private:
	bool _issuerCode;
    bool _pin;
    bool _accessCondition5;
    bool _accessCondition4;
    bool _accessCondition3;
    bool _accessCondition2;
    bool _accessCondition1;
};