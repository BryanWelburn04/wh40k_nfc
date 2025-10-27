#if !defined(AFX_ACR122U_H__9FA8A8BA_633E_4717_A7DB_73AD020A1758__INCLUDED_)
#define AFX_ACR122U_H__9FA8A8BA_633E_4717_A7DB_73AD020A1758__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "PcscReader.h"
#include "PcscException.h"
#include "AcsCommon.h"
#include "Apdu.h"

enum ANTENNA_STATUS
{
    ANTENNA_STATUS_ON = 0x01,
    ANTENNA_STATUS_OFF = 0x00
};

enum BUZZER_OUTPUT
{
    BUZZER_OUTPUT_ON = 0xFF,
    BUZZER_OUTPUT_OFF = 0x00
};

enum PARAMETER_OPTION
{
	PARAMETER_OPTION_SKIP = 0x00,
	PARAMETER_OPTION_DETECT = 0x01
};

enum POLLING_INTERVAL
{ 
    POLLING_INTERVAL_MS_500,  
	POLLING_INTERVAL_MS_250	
};

enum PICC_POLLING
{
	PICC_POLLING_ENABLED = 0x01,
	PICC_POLLING_DISABLED = 0x00
};

enum ATS_GENERATION
{
	ATS_GENERATION_ENABLED = 0x01,
	ATS_GENERATION_DISABLED = 0x00
};

enum LED_STATE
{
    LED_STATE_ON = 0x01,
    LED_STATE_OFF = 0x00
};

enum LED_STATE_MASK
{
    LED_STATE_MASK_ON = 0x01,
    LED_STATE_MASK_OFF = 0x00
};

enum INITIAL_LED_BLINKING_STATE
{
    INITIAL_LED_BLINKING_STATE_ON = 0x01,
    INITIAL_LED_BLINKING_STATE_OFF = 0x00
};

enum LED_BLINKING_MASK
{
    LED_BLINKING_MASK_ON = 0x01,
    LED_BLINKING_MASK_OFF = 0x00
};

enum LED_BUZZER_BEHAVIOR
{ 
    LED_BUZZER_BEHAVIOR_ENABLED = 0x01,
    LED_BUZZER_BEHAVIOR_DISABLED = 0x00
};

enum LINK_TO_BUZZER
{
	LINK_TO_BUZZER_OFF = 0x00,
	LINK_TO_BUZZER_T1_DURATION = 0x01,
	LINK_TO_BUZZER_T2_DURATION = 0x02,
	LINK_TO_BUZZER_T1_AND_T2_DURATION = 0x03
};

class LedAndBuzzerControl
{

public:
	LedAndBuzzerControl();
	LedAndBuzzerControl(byte uLedState);
	byte getRawLedStatus();
	
	LED_STATE getRedLedState() { return eRedLedState_; }
	void setRedLedState(LED_STATE eRedLedState) { eRedLedState_ = eRedLedState; }

    LED_STATE getGreenLedState() { return eGreenLedState_; }
	void setGreenLedState(LED_STATE eGreenLedState) { eGreenLedState_ = eGreenLedState; }

	LED_STATE_MASK getRedLedStateMask() { return eRedLedStateMask_; }
	void setRedLedStateMask(LED_STATE_MASK eRedLedStateMask) { eRedLedStateMask_ = eRedLedStateMask; }

	LED_STATE_MASK getGreenLedStateMask() { return eGreenLedStateMask_; }
	void setGreenLedStateMask(LED_STATE_MASK eGreenLedStateMask) { eGreenLedStateMask_ = eGreenLedStateMask; }

	INITIAL_LED_BLINKING_STATE getRedLedBlinkingState() { return eRedLedBlinkingState_; }
	void setRedLedBlinkingState(INITIAL_LED_BLINKING_STATE eRedInitialLedBlinkingState) { eRedLedBlinkingState_ = eRedInitialLedBlinkingState; }

	INITIAL_LED_BLINKING_STATE getGreenLedBlinkingState() { return eGreenLedBlinkingState_; }
	void setGreenLedBlinkingState(INITIAL_LED_BLINKING_STATE eGreenInitialLedBlinkingState) { eGreenLedBlinkingState_ = eGreenInitialLedBlinkingState; }

	LED_BLINKING_MASK getRedLedBlinkingMask() { return eRedLedBlinkingMask_; }
	void setRedLedBlinkingMask(LED_BLINKING_MASK eRedLedBlinkingMask) { eRedLedBlinkingMask_ = eRedLedBlinkingMask; }

	LED_BLINKING_MASK getGreenLedBlinkingMask() { return eGreenLedBlinkingMask_; }
	void setGreenLedBlinkingMask(LED_BLINKING_MASK eGreenLedBlinkingMask) { eGreenLedBlinkingMask_ = eGreenLedBlinkingMask; }

    byte* getBlinkingDuration() { return aBlinkingDuration_; }
	void setBlinkingDuration(byte* pBlinkingDuration) { memcpy(aBlinkingDuration_,pBlinkingDuration,sizeof(aBlinkingDuration_)); }

private:
    LED_STATE eRedLedState_;
    LED_STATE eGreenLedState_;
    LED_STATE_MASK eRedLedStateMask_;
    LED_STATE_MASK eGreenLedStateMask_;
    INITIAL_LED_BLINKING_STATE eRedLedBlinkingState_;
    INITIAL_LED_BLINKING_STATE eGreenLedBlinkingState_;
    LED_BLINKING_MASK eRedLedBlinkingMask_;
    LED_BLINKING_MASK eGreenLedBlinkingMask_;
    byte aBlinkingDuration_[4];
};

class PiccOperatingParameter
{ 
	
public:
	PiccOperatingParameter();
	PiccOperatingParameter(byte uRawOperatingParameter);
	byte getRawOperatingParameter();

	PARAMETER_OPTION getIso14443TypeA() { return eIso14443TypeA_; }
	void setIso14443TypeA(PARAMETER_OPTION eParameterOption) { eIso14443TypeA_ = eParameterOption; }

	PARAMETER_OPTION getIso14443TypeB() { return eIso14443TypeB_; }
	void setIso14443TypeB(PARAMETER_OPTION eParameterOption) { eIso14443TypeB_ = eParameterOption; }

	PARAMETER_OPTION getFelica212() { return eFelica212_; }
	void setFelica212(PARAMETER_OPTION eParameterOption) { eFelica212_ = eParameterOption; }

	PARAMETER_OPTION getFelica424() { return eFelica424_; }
	void setFelica424(PARAMETER_OPTION eParameterOption) { eFelica424_ = eParameterOption; }

	PARAMETER_OPTION getTopaz() { return eTopaz_; }
	void setTopaz(PARAMETER_OPTION eParameterOption) { eTopaz_ = eParameterOption; }

	POLLING_INTERVAL getPollingInterval() { return ePollingInterval_; }
	void setPollingInterval(POLLING_INTERVAL ePollingInterval) { ePollingInterval_ = ePollingInterval; }

	PICC_POLLING getAutoPiccPolling() { return ePiccPolling_; }
	void setAutoPiccPolling(PICC_POLLING ePiccPolling) { ePiccPolling_ = ePiccPolling; }

	ATS_GENERATION getAutoAtsGeneration() { return eAtsGeneration_; }
	void setAutoAtsGeneration(ATS_GENERATION eAtsGeneration) { eAtsGeneration_ = eAtsGeneration; }

private:
	PARAMETER_OPTION eIso14443TypeA_;
	PARAMETER_OPTION eIso14443TypeB_;
	PARAMETER_OPTION eFelica212_;
	PARAMETER_OPTION eFelica424_;
	PARAMETER_OPTION eTopaz_;
	POLLING_INTERVAL ePollingInterval_;
	PICC_POLLING ePiccPolling_;
	ATS_GENERATION eAtsGeneration_;
};

class AutoPiccPollingSettings
{
public:
	AutoPiccPollingSettings();
	AutoPiccPollingSettings(byte uRawPiccPollingSetting);
	void setRawPollingSetting(byte uRawPollingSetting);
	byte getRawPollingSettings();

	PICC_POLLING getAutoPiccPolling() { return eAutoPiccPolling_; }
	void setAutoPiccPolling(PICC_POLLING ePollingSetting) { eAutoPiccPolling_ = ePollingSetting; }

	ATS_GENERATION getAutoAtsGeneration() { return eAutoAtsGeneration_; }
	void setAutoAtsGeneration(ATS_GENERATION eAtsGeneration) { eAutoAtsGeneration_ = eAtsGeneration; }

	POLLING_INTERVAL getPollingInterval() { return ePollingInterval_; }
	void setPollingInterval(POLLING_INTERVAL ePollingInterval) { ePollingInterval_ = ePollingInterval; }

	PARAMETER_OPTION getDetectFelica212() { return eDetectFelica212_; }
	void setDetectFelica212(PARAMETER_OPTION eParameterOption) { eDetectFelica212_ = eParameterOption; }

	PARAMETER_OPTION getDetectFelica424() { return eDetectFelica424_; }
	void setDetectFelica424(PARAMETER_OPTION eParameterOption) { eDetectFelica424_ = eParameterOption; }

	PARAMETER_OPTION getDetectTopaz() { return eDetectTopaz_; }
	void setDetectTopaz(PARAMETER_OPTION eParameterOption) { eDetectTopaz_ = eParameterOption; }

	PARAMETER_OPTION getDetectIso14443TypeB() { return eDetectIso14443TypeB_; }
	void setDetectIso14443TypeB(PARAMETER_OPTION eParameterOption) { eDetectIso14443TypeB_ = eParameterOption; }

	PARAMETER_OPTION getDetectIso14443TypeA() { return eDetectIso14443TypeA_; }
	void setDetectIso14443TypeA(PARAMETER_OPTION eParameterOption) { eDetectIso14443TypeA_ = eParameterOption; }

private:

	PICC_POLLING eAutoPiccPolling_;
	ATS_GENERATION eAutoAtsGeneration_;
	POLLING_INTERVAL ePollingInterval_;
	PARAMETER_OPTION eDetectFelica212_;
	PARAMETER_OPTION eDetectFelica424_;
	PARAMETER_OPTION eDetectTopaz_;
	PARAMETER_OPTION eDetectIso14443TypeB_;
	PARAMETER_OPTION eDetectIso14443TypeA_;
}; 

class Acr122u
{
public:
	Acr122u();
	Acr122u(PcscReader* pPcscReader);
	virtual ~Acr122u();

	CString getFirmwareVersion();
	LedAndBuzzerControl* setLedStatus(LedAndBuzzerControl* pLedAndBuzzerControl);
	void setBuzzerOutput(byte uStatus);
	PiccOperatingParameter* getPiccOperatingParameter();
	PiccOperatingParameter* setPiccOperatingParameter(PiccOperatingParameter* pPiccOperatingParameter);
	byte* getStatus(int& iReceivedLength);
	void setAntenna(ANTENNA_STATUS eAntennaStatus);
	void setTimeOutParameter(byte uTimeOutParameter);
	
private:
	static const int PARAMETER_BIT_FLAG_INDEX;
	PcscReader *pPcscReader_;
};

#endif
