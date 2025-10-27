#include "stdafx.h"
#include "Acr122u.h"

const int Acr122u::PARAMETER_BIT_FLAG_INDEX = 0x01;

LedAndBuzzerControl::LedAndBuzzerControl()
{
	eRedLedState_ = LED_STATE_OFF;
	eGreenLedState_ = LED_STATE_OFF;
	eRedLedStateMask_ = LED_STATE_MASK_OFF;
	eGreenLedStateMask_ = LED_STATE_MASK_OFF;
	eRedLedBlinkingState_ = INITIAL_LED_BLINKING_STATE_OFF;
	eGreenLedBlinkingState_ = INITIAL_LED_BLINKING_STATE_OFF;
	eRedLedBlinkingMask_ = LED_BLINKING_MASK_OFF;
	eGreenLedBlinkingMask_ = LED_BLINKING_MASK_OFF;
}

LedAndBuzzerControl::LedAndBuzzerControl(byte uLedState)
{
    if ((uLedState & 0x01) == 0x01)
        eRedLedState_ = LED_STATE_ON;

    if ((uLedState & 0x02) == 0x02)
        eGreenLedState_ = LED_STATE_ON;

    if ((uLedState & 0x04) == 0x04)
        eRedLedStateMask_ = LED_STATE_MASK_ON;

    if ((uLedState & 0x08) == 0x08)
        eGreenLedStateMask_ = LED_STATE_MASK_ON;

    if ((uLedState & 0x10) == 0x10)
        eRedLedBlinkingState_ = INITIAL_LED_BLINKING_STATE_ON;

    if ((uLedState & 0x20) == 0x20)
        eGreenLedBlinkingState_ = INITIAL_LED_BLINKING_STATE_ON;

    if ((uLedState & 0x40) == 0x40)
        eRedLedBlinkingMask_ = LED_BLINKING_MASK_ON;

    if ((uLedState & 0x80) == 0x80)
        eGreenLedBlinkingMask_ = LED_BLINKING_MASK_ON;	
}

byte LedAndBuzzerControl::getRawLedStatus()
{
    byte uLedStatus = 0x00;

    if (getRedLedState() == LED_STATE_ON)
        uLedStatus |= 0x01;

    if (getGreenLedState() == LED_STATE_ON)
        uLedStatus |= 0x02;

    if (getRedLedStateMask() == LED_STATE_MASK_ON)
        uLedStatus |= 0x04;

    if(getGreenLedStateMask() == LED_STATE_MASK_ON)
        uLedStatus |= 0x08;

    if(getRedLedBlinkingState() == INITIAL_LED_BLINKING_STATE_ON)
        uLedStatus |= 0x10;

    if(getGreenLedBlinkingState() == INITIAL_LED_BLINKING_STATE_ON)
        uLedStatus |= 0x20;

    if(getRedLedBlinkingMask() == LED_BLINKING_MASK_ON)
        uLedStatus |= 0x40;

    if(getGreenLedBlinkingMask() == LED_BLINKING_MASK_ON)
        uLedStatus |= 0x80;

    return uLedStatus;
}

PiccOperatingParameter::PiccOperatingParameter()
{ 
	eIso14443TypeA_ = PARAMETER_OPTION_SKIP;
	eIso14443TypeB_ = PARAMETER_OPTION_SKIP;
	eFelica212_ = PARAMETER_OPTION_SKIP;
	eFelica424_ = PARAMETER_OPTION_SKIP;
	eTopaz_ = PARAMETER_OPTION_SKIP;
	ePollingInterval_ = POLLING_INTERVAL_MS_500;
	ePiccPolling_ = PICC_POLLING_DISABLED;
	eAtsGeneration_ = ATS_GENERATION_DISABLED;
}

PiccOperatingParameter::PiccOperatingParameter(byte uRawOperatingParameter)
{
	ePollingInterval_ = POLLING_INTERVAL_MS_500;

    if ((uRawOperatingParameter & 0x01) == 0x01)
        setIso14443TypeA(PARAMETER_OPTION_DETECT);

    if ((uRawOperatingParameter & 0x02) == 0x02)
        setIso14443TypeB(PARAMETER_OPTION_DETECT);

    if ((uRawOperatingParameter & 0x04) == 0x04)
        setTopaz(PARAMETER_OPTION_DETECT);

    if ((uRawOperatingParameter & 0x08) == 0x08)
        setFelica212(PARAMETER_OPTION_DETECT);

    if ((uRawOperatingParameter & 0x10) == 0x10)
        setFelica424(PARAMETER_OPTION_DETECT);

    if ((uRawOperatingParameter & 0x20) == 0x20)
        setPollingInterval(POLLING_INTERVAL_MS_250);

    if ((uRawOperatingParameter & 0x40) == 0x40)
        setAutoAtsGeneration(ATS_GENERATION_ENABLED);

    if ((uRawOperatingParameter & 0x80) == 0x80)
        setAutoPiccPolling(PICC_POLLING_ENABLED);
}

byte PiccOperatingParameter::getRawOperatingParameter()
{
    byte uOperatingParameter = 0x00;

    if (getIso14443TypeA() == PARAMETER_OPTION_DETECT)
        uOperatingParameter |= 0x01;

    if (getIso14443TypeB() == PARAMETER_OPTION_DETECT)
        uOperatingParameter |= 0x02;

    if (getTopaz() == PARAMETER_OPTION_DETECT)
        uOperatingParameter |= 0x04;

    if (getFelica212() == PARAMETER_OPTION_DETECT)
        uOperatingParameter |= 0x08;

    if (getFelica424() == PARAMETER_OPTION_DETECT)
        uOperatingParameter |= 0x10;

    if (getPollingInterval() == POLLING_INTERVAL_MS_250)
        uOperatingParameter |= 0x20;

    if (getAutoAtsGeneration() == ATS_GENERATION_ENABLED)
        uOperatingParameter |= 0x40;

    if (getAutoPiccPolling() == PICC_POLLING_ENABLED)
        uOperatingParameter |= 0x80;

    return uOperatingParameter;
}

AutoPiccPollingSettings::AutoPiccPollingSettings()
{ 
    eAutoPiccPolling_ = PICC_POLLING_DISABLED;
    eAutoAtsGeneration_ = ATS_GENERATION_DISABLED;
    ePollingInterval_ = POLLING_INTERVAL_MS_250;
    eDetectFelica212_ = PARAMETER_OPTION_DETECT;
    eDetectFelica424_ = PARAMETER_OPTION_DETECT;
    eDetectTopaz_ = PARAMETER_OPTION_DETECT;
    eDetectIso14443TypeB_ = PARAMETER_OPTION_DETECT;
    eDetectIso14443TypeA_ = PARAMETER_OPTION_DETECT;
}

AutoPiccPollingSettings::AutoPiccPollingSettings(byte uRawPiccPollingSetting)
{
    eAutoPiccPolling_ = PICC_POLLING_DISABLED;
    eAutoAtsGeneration_ = ATS_GENERATION_DISABLED;
    ePollingInterval_ = POLLING_INTERVAL_MS_250;
    eDetectFelica212_ = PARAMETER_OPTION_DETECT;
    eDetectFelica424_ = PARAMETER_OPTION_DETECT;
    eDetectTopaz_ = PARAMETER_OPTION_DETECT;
    eDetectIso14443TypeB_ = PARAMETER_OPTION_DETECT;
    eDetectIso14443TypeA_ = PARAMETER_OPTION_DETECT;

    setRawPollingSetting(uRawPiccPollingSetting);
}

void AutoPiccPollingSettings::setRawPollingSetting(byte uRawPollingSetting)
{
    if ((uRawPollingSetting & 0x01) == 0x01)
        setDetectIso14443TypeA(PARAMETER_OPTION_DETECT);

    if ((uRawPollingSetting & 0x02) == 0x02)
        setDetectIso14443TypeB(PARAMETER_OPTION_DETECT);

    if ((uRawPollingSetting & 0x04) == 0x04)
        setDetectTopaz(PARAMETER_OPTION_DETECT);

    if ((uRawPollingSetting & 0x08) == 0x08)
        setDetectFelica212(PARAMETER_OPTION_DETECT);

    if ((uRawPollingSetting & 0x10) == 0x10)
        setDetectFelica424(PARAMETER_OPTION_DETECT);

	if ((uRawPollingSetting & 0x20) == 0x20)
        setPollingInterval(POLLING_INTERVAL_MS_500);

    if ((uRawPollingSetting & 0x40) == 0x40)
        setAutoAtsGeneration(ATS_GENERATION_ENABLED);

    if ((uRawPollingSetting & 0x80) == 0x80)
        setAutoPiccPolling(PICC_POLLING_ENABLED);
}

byte AutoPiccPollingSettings::getRawPollingSettings()
{
    byte uRawPollingSetting = 0x00;

    if (getDetectIso14443TypeA() == PARAMETER_OPTION_DETECT)
        uRawPollingSetting |= 0x01;

    if (getDetectIso14443TypeB() == PARAMETER_OPTION_DETECT)
        uRawPollingSetting |= 0x02;

    if (getDetectTopaz() == PARAMETER_OPTION_DETECT)
        uRawPollingSetting |= 0x04;

    if (getDetectFelica212() == PARAMETER_OPTION_DETECT)
        uRawPollingSetting |= 0x08;

    if (getDetectFelica424() == PARAMETER_OPTION_DETECT)
        uRawPollingSetting |= 0x10;

    if (getPollingInterval() == POLLING_INTERVAL_MS_500)
        uRawPollingSetting |= 0x20;

    if (getAutoAtsGeneration() == ATS_GENERATION_ENABLED)
        uRawPollingSetting |= 0x40;

    if (getAutoPiccPolling() == PICC_POLLING_ENABLED)
        uRawPollingSetting |= 0x80;

    return uRawPollingSetting;
}

Acr122u::Acr122u() {}

Acr122u::Acr122u(PcscReader* pPcscReader)
{
	pPcscReader_ = pPcscReader;
}

Acr122u::~Acr122u()
{
}

CString Acr122u::getFirmwareVersion()
{
	Apdu *pApdu = new Apdu();

	CString sFirmwareVersion = "";
	char cFirmwareVersion;
	int iIndex;

	uint8 aCommand[] = { 0xFF, 0x00, 0x48, 0x00, 0x00 };
	uint8 aReceivedData[256] = { 0 };
	uint8 aStatusWord[2] = { 0 };
    
	pApdu->setCommand(aCommand);
	pApdu->setSendLen(5);
	pApdu->setReceiveData(aReceivedData);
	pApdu->setReceiveLen(256);
	pApdu->setStatusWord(aStatusWord);

	try
	{
		pPcscReader_->sendControlCommand(pApdu);
	}
	catch(PcscException cException)
	{
		delete pApdu;
		throw cException;
	}

	for (iIndex = 0; iIndex < pApdu->getReceiveLen(); iIndex++)
	{
		cFirmwareVersion = aReceivedData[iIndex];
		sFirmwareVersion = sFirmwareVersion + cFirmwareVersion;
	}

	delete pApdu; 
	
    return sFirmwareVersion;
}

PiccOperatingParameter* Acr122u::getPiccOperatingParameter()
{
	Apdu *pApdu = new Apdu();
	PiccOperatingParameter *pPiccOperatingParameter;

	uint8 aCommand[] = { 0xFF, 0x00, 0x50, 0x00, 0x00 };
	uint8 aReceivedData[255] = { 0 };
	uint8 aStatusWord[2] = { 0 };
    
	pApdu->setCommand(aCommand);
	pApdu->setSendLen(sizeof(aCommand));
	pApdu->setReceiveData(aReceivedData);
	pApdu->setReceiveLen(sizeof(aReceivedData));
	pApdu->setStatusWord(aStatusWord);

	try
	{
		pPcscReader_->sendControlCommand(pApdu);
	}
	catch(PcscException cException)
	{
		delete pApdu;
		throw cException;
	}

	pPiccOperatingParameter = new PiccOperatingParameter(pApdu->getReceiveData()[PARAMETER_BIT_FLAG_INDEX]);

	delete pApdu;

	return pPiccOperatingParameter;
}

PiccOperatingParameter* Acr122u::setPiccOperatingParameter(PiccOperatingParameter* pPiccOperatingParameter)
{
	Apdu *pApdu = new Apdu();
	PiccOperatingParameter *pReturnedPiccOperatingParameter;

	uint8 aCommand[] = { 0xFF, 0x00, 0x51, pPiccOperatingParameter->getRawOperatingParameter(), 0x00 };
	uint8 aReceivedData[255] = { 0 };
	uint8 aStatusWord[2] = { 0 };
    
	pApdu->setCommand(aCommand);
	pApdu->setSendLen(sizeof(aCommand));
	pApdu->setReceiveData(aReceivedData);
	pApdu->setReceiveLen(sizeof(aReceivedData));
	pApdu->setStatusWord(aStatusWord);

	try
	{
		pPcscReader_->sendControlCommand(pApdu);
	}
	catch(PcscException cException)
	{
		delete pApdu;
		throw cException;
	}

	pReturnedPiccOperatingParameter = new PiccOperatingParameter(pApdu->getReceiveData()[PARAMETER_BIT_FLAG_INDEX]);

	delete pApdu;

	return pReturnedPiccOperatingParameter;
}

byte* Acr122u::getStatus(int& iReceivedLength)
{
	Apdu *pApdu = new Apdu();
	byte *pStatusBytes;
	int iReceivedLen = 0;

	uint8 aCommand[] = { 0xFF, 0x00, 0x00, 0x00, 0x02 };
	uint8 aCommandData[] = { 0xD4, 0x04 };
	uint8 aReceivedData[255] = { 0 };
	uint8 aStatusWord[2] = { 0 };
    
	pApdu->setCommand(aCommand);
	pApdu->setSendData(aCommandData);
	pApdu->setSendLen(sizeof(aCommand) + sizeof(aCommandData));
	pApdu->setReceiveData(aReceivedData);
	pApdu->setReceiveLen(sizeof(aReceivedData));
	pApdu->setStatusWord(aStatusWord);

	try
	{
		pPcscReader_->sendControlCommand(pApdu);
	}
	catch(PcscException cException)
	{
		delete pApdu;
		throw cException;
	}

	iReceivedLen = pApdu->getReceiveLen();
	pStatusBytes = new byte[iReceivedLen];
	memcpy(pStatusBytes,pApdu->getReceiveData(),iReceivedLen);

	iReceivedLength = iReceivedLen;

	delete pApdu;

    return pStatusBytes;
}

void Acr122u::setAntenna(ANTENNA_STATUS eAntennaStatus)
{

	Apdu *pApdu = new Apdu();

	uint8 aCommand[] = { 0xFF, 0x00, 0x00, 0x00, 0x04 };
	uint8 aCommandData[] = { 0xD4, 0x32, 0x01, (byte) eAntennaStatus };
	uint8 aReceivedData[255] = { 0 };
	uint8 aStatusWord[2] = { 0 };
    
	pApdu->setCommand(aCommand);
	pApdu->setSendData(aCommandData);
	pApdu->setSendLen(sizeof(aCommand) + sizeof(aCommandData));
	pApdu->setReceiveData(aReceivedData);
	pApdu->setReceiveLen(sizeof(aReceivedData));
	pApdu->setStatusWord(aStatusWord);

	try
	{
		pPcscReader_->sendControlCommand(pApdu);
	}
	catch(PcscException cException)
	{
		delete pApdu;
		throw cException;
	}

	delete pApdu;
}

void Acr122u::setTimeOutParameter(byte uTimeOutParameter)
{
	Apdu *pApdu = new Apdu();

	uint8 aCommand[] = { 0xFF, 0x00, 0x41, uTimeOutParameter, 0x00};
	uint8 aReceivedData[2] = { 0 };
	uint8 aStatusWord[2] = { 0 };
    
	pApdu->setCommand(aCommand);
	pApdu->setSendLen(sizeof(aCommand));
	pApdu->setReceiveData(aReceivedData);
	pApdu->setReceiveLen(sizeof(aReceivedData));
	pApdu->setStatusWord(aStatusWord);

	try
	{
		pPcscReader_->sendControlCommand(pApdu);
	}
	catch(PcscException cException)
	{
		delete pApdu;
		throw cException;
	}

	delete pApdu;
}

void Acr122u::setBuzzerOutput(byte uStatus)
{
	Apdu *pApdu = new Apdu();

	uint8 aCommand[] = { 0xFF, 0x00, 0x52, uStatus, 0x00};
	uint8 aReceivedData[2] = { 0 };
	uint8 aStatusWord[2] = { 0 };
    
	pApdu->setCommand(aCommand);
	pApdu->setSendLen(sizeof(aCommand));
	pApdu->setReceiveData(aReceivedData);
	pApdu->setReceiveLen(sizeof(aReceivedData));
	pApdu->setStatusWord(aStatusWord);

	try
	{
		pPcscReader_->sendControlCommand(pApdu);
	}
	catch(PcscException cException)
	{
		delete pApdu;
		throw cException;
	}

	delete pApdu;
}

LedAndBuzzerControl* Acr122u::setLedStatus(LedAndBuzzerControl* pLedAndBuzzerControl)
{
	Apdu *pApdu = new Apdu();
	LedAndBuzzerControl *pNewLedAndBuzzerControl;

	uint8 aCommand[] = { 0xFF, 0x00, 0x40, pLedAndBuzzerControl->getRawLedStatus(), 0x04};
	uint8 aCommandData[] = { pLedAndBuzzerControl->getBlinkingDuration()[0], 
							 pLedAndBuzzerControl->getBlinkingDuration()[1],
							 pLedAndBuzzerControl->getBlinkingDuration()[2],
							 pLedAndBuzzerControl->getBlinkingDuration()[3] };

	uint8 aReceivedData[2] = { 0 };
	uint8 aStatusWord[2] = { 0 };
    
	pApdu->setCommand(aCommand);
	pApdu->setSendData(aCommandData);
	pApdu->setSendLen(sizeof(aCommand) + sizeof(aCommandData));
	pApdu->setReceiveData(aReceivedData);
	pApdu->setReceiveLen(sizeof(aReceivedData));
	pApdu->setStatusWord(aStatusWord);

	try
	{
		pPcscReader_->sendControlCommand(pApdu);
	}
	catch(PcscException cException)
	{
		delete pApdu;
		throw cException;
	}

	pNewLedAndBuzzerControl = new LedAndBuzzerControl(pApdu->getReceiveData()[PARAMETER_BIT_FLAG_INDEX]);

	delete pApdu;

	return pNewLedAndBuzzerControl;
}
