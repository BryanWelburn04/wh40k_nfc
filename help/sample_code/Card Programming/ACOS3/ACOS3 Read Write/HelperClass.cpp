//===========================================================================================
// 
//  Author          : Valerie C. Dio
// 
//  File            : HelperClass.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : Contains helper classes.
// 
//  Date            : October 12, 2012
// 
//  Revision Traile : [Author] / [Date of modification] / [Details of Modifications done]
// 
// 
//=========================================================================================

#include "stdafx.h"
#include "ACOS Sample Codes.h"
#include "HelperClass.h"
#include "Helper.h"


void SecurityAttribute::setIssuerCode(bool issuerCode)
{
	_issuerCode = issuerCode;
}

void SecurityAttribute::setPin(bool pin)
{
	_pin = pin;
}

void SecurityAttribute::setAccessCondition5(bool accessCondition5)
{
	_accessCondition5 = accessCondition5;
}

void SecurityAttribute::setAccessCondition4(bool accessCondition4)
{
	_accessCondition4 = accessCondition4;
}

void SecurityAttribute::setAccessCondition3(bool accessCondition3)
{
	_accessCondition3 = accessCondition3;
}

void SecurityAttribute::setAccessCondition2(bool accessCondition2)
{
	_accessCondition2 = accessCondition2;
}

void SecurityAttribute::setAccessCondition1(bool accessCondition1)
{
	_accessCondition1 = accessCondition1;
}

void SecurityAttribute::securityAttribute(byte securityAttribute)
{
	if ((securityAttribute & 0x01) == 0x80)
        _issuerCode = true;

    if ((securityAttribute & 0x01) == 0x40)
        _pin = true;

    if ((securityAttribute & 0x01) == 0x20)
        _accessCondition5 = true;

    if ((securityAttribute & 0x01) == 0x10)
        _accessCondition4 = true;

    if ((securityAttribute & 0x01) == 0x08)
        _accessCondition3 = true;

    if ((securityAttribute & 0x01) == 0x04)
        _accessCondition2 = true;

    if ((securityAttribute & 0x01) == 0x02)
        _accessCondition1 = true;
}

byte SecurityAttribute::getRawValue()
{
	byte rawValue = 0x00;

    if (_accessCondition1)
        rawValue |= 0x02;

    if (_accessCondition2)
        rawValue |= 0x04;

    if (_accessCondition3)
        rawValue |= 0x08;

    if (_accessCondition4)
        rawValue |= 0x10;

    if (_accessCondition5)
        rawValue |= 0x20;

    if (_pin)
        rawValue |= 0x40;

    if (_issuerCode)
        rawValue |= 0x80;

    return rawValue;
}

void InquireAccountResponse::setMac(byte* mac)
{
	_mac = mac;
}

void InquireAccountResponse::setTransactionType(byte transactionType)
{
	_transactionType = transactionType;
}

void InquireAccountResponse::setBalance(int balance)
{
	Helper* helper = new Helper();
	_balance = helper->intToByte(balance);
}

void InquireAccountResponse::setAtref(byte* atref)
{
	_atref = atref;
}

void InquireAccountResponse::setMaximumBalanceAllowed(int maximumBalanceAllowed)
{
	Helper* helper = new Helper();
	_maximumBalanceAllowed = helper->intToByte(maximumBalanceAllowed);
}

void InquireAccountResponse::setTtrefc(byte* ttrefc)
{
	_ttrefc = ttrefc;
}

void InquireAccountResponse::setTtrefd(byte* ttrefd)
{
	_ttrefd = ttrefd;
}

byte* InquireAccountResponse::inquireAccountResponse(byte* rawDataFromCard)
{
	if(rawDataFromCard == NULL || sizeof(rawDataFromCard) <25)
		throw "Data from card has invalid lenght";

	int i;
	for (i = 0; i < 3; i++)
			_mac[i] = rawDataFromCard[i];
	_transactionType = rawDataFromCard[4];
	for (i = 0; i < 2; i++)
			_balance[i] = rawDataFromCard[i+5];
	for (i = 0; i < 5; i++)
			_atref[i] = rawDataFromCard[i+8];
	for (i = 0; i < 2; i++)
			_maximumBalanceAllowed[i] = rawDataFromCard[i+14];
	for (i = 0; i < 3; i++)
			_ttrefc[i] = rawDataFromCard[i+17];
	for (i = 0; i < 3; i++)
			_ttrefd[i] = rawDataFromCard[i+21];

	return rawDataFromCard;
}

void OptionRegister::setEnableAccount(bool enableAccount)
{
	_enableAccount = enableAccount;
}

void OptionRegister::setEnableTripleDes(bool enableTripleDes)
{
	_enableTripleDes = enableTripleDes;
}

void OptionRegister::setEnableChangePinCommand(bool enableChangePinCommand)
{
	_enableChangePinCommand = enableChangePinCommand;
}

void OptionRegister::setEnableDebitMac(bool enableDebitMac)
{
	_enableDebitMac = enableDebitMac;
}

void OptionRegister::setRequirePinDuringDebit(bool requirePinDuringDebit)
{
	_requirePinDuringDebit = requirePinDuringDebit;
}

void OptionRegister::setEnableRevokeDebitCommand(bool enableRevokeDebitCommand)
{
	_enableRevokeDebitCommand = enableRevokeDebitCommand;
}

void OptionRegister::setRequireMutualAuthenticationOnAccountTransaction(bool requireMutualAuthenticationOnAccountTransaction)
{
	_requireMutualAuthenticationOnAccountTransaction = requireMutualAuthenticationOnAccountTransaction;
}

void OptionRegister::setRequireMutualAuthenticationOnInquireAccount(bool requireMutualAuthenticationOnInquireAccount)
{
	_requireMutualAuthenticationOnInquireAccount = requireMutualAuthenticationOnInquireAccount;
}

void OptionRegister::optionRegister(byte optionRegisterByte)
{
	if ((optionRegisterByte & 0x01) == 0x01)
        _enableAccount = true;

    if ((optionRegisterByte & 0x01) == 0x02)
        _enableTripleDes = true;

    if ((optionRegisterByte & 0x01) == 0x04)
        _enableChangePinCommand = true;

    if ((optionRegisterByte & 0x01) == 0x08)
        _enableDebitMac = true;

    if ((optionRegisterByte & 0x01) == 0x10)
        _requirePinDuringDebit = true;

    if ((optionRegisterByte & 0x01) == 0x20)
        _enableRevokeDebitCommand = true;

    if ((optionRegisterByte & 0x01) == 0x40)
        _requireMutualAuthenticationOnAccountTransaction = true;

    if ((optionRegisterByte & 0x01) == 0x80)
        _requireMutualAuthenticationOnInquireAccount = true;
}

byte OptionRegister::getRawValue()
{
	byte rawValue = 0x00;
	
    if (_enableAccount)
        rawValue |= 0x01;

    if (_enableTripleDes)
        rawValue |= 0x02;

    if (_enableChangePinCommand)
        rawValue |= 0x04;

    if (_enableDebitMac)
        rawValue |= 0x08;

    if (_requirePinDuringDebit)
        rawValue |= 0x10;

    if (_enableRevokeDebitCommand)
        rawValue |= 0x20;

    if (_requireMutualAuthenticationOnAccountTransaction)
        rawValue |= 0x40;

    if (_requireMutualAuthenticationOnInquireAccount)
        rawValue |= 0x80;

    return rawValue;
}

void SecurityOptionRegister::setIssuerCode(bool issuerCode)
{
	_issuerCode = issuerCode;
}

void SecurityOptionRegister::setPin(bool pin)
{
	_pin = pin;
}

void SecurityOptionRegister::setAccessCondition5(bool accessCondition5)
{
	_accessCondition5 = accessCondition5;
}

void SecurityOptionRegister::setAccessCondition4(bool accessCondition4)
{
	_accessCondition4 = accessCondition4;
}

void SecurityOptionRegister::setAccessCondition3(bool accessCondition3)
{
	_accessCondition3 = accessCondition3;
}

void SecurityOptionRegister::setAccessCondition2(bool accessCondition2)
{
	_accessCondition2 = accessCondition2;
}

void SecurityOptionRegister::setAccessCondition1(bool accessCondition1)
{
	_accessCondition1 = accessCondition1;
}

void SecurityOptionRegister::securityAttribute(byte securityAttribute)
{
	if ((securityAttribute & 0x01) == 0x80)
        _issuerCode = true;

    if ((securityAttribute & 0x01) == 0x40)
        _pin = true;

    if ((securityAttribute & 0x01) == 0x20)
        _accessCondition5 = true;

    if ((securityAttribute & 0x01) == 0x10)
        _accessCondition4 = true;

    if ((securityAttribute & 0x01) == 0x08)
        _accessCondition3 = true;

    if ((securityAttribute & 0x01) == 0x04)
        _accessCondition2 = true;

    if ((securityAttribute & 0x01) == 0x02)
        _accessCondition1 = true;
}

byte SecurityOptionRegister::getRawValue()
{
	byte rawValue = 0x00;

    if (_accessCondition1)
        rawValue |= 0x02;

    if (_accessCondition2)
        rawValue |= 0x04;

    if (_accessCondition3)
        rawValue |= 0x08;

    if (_accessCondition4)
        rawValue |= 0x10;

    if (_accessCondition5)
        rawValue |= 0x20;

    if (_pin)
        rawValue |= 0x40;

    if (_issuerCode)
        rawValue |= 0x80;

    return rawValue;
}