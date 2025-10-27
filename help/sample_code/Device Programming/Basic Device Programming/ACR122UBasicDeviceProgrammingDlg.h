#if !defined(AFX_ACR122UBASICDEVICEPROGRAMMINGDLG_H__52ABA0E4_BBA3_4F25_983D_8A31C9CC18EB__INCLUDED_)
#define AFX_ACR122UBASICDEVICEPROGRAMMINGDLG_H__52ABA0E4_BBA3_4F25_983D_8A31C9CC18EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "PcscReader.h"
#include "PcscException.h"
#include "Acr122u.h"

void onReceiveCommand(const byte* pReceiveData, const int iLength);
void onSendCommand(const byte* pSendData, const int iLength);

class CACR122UBasicDeviceProgrammingDlg : public CDialog
{
public:

	CACR122UBasicDeviceProgrammingDlg(CWnd* pParent = NULL);	// standard constructor
	~CACR122UBasicDeviceProgrammingDlg();

	void resetControls();
	void enableControls(bool bIsEnabled);
	void addMessageToLogs(CString sMessage);
// Dialog Data
	//{{AFX_DATA(CACR122UBasicDeviceProgrammingDlg)
	enum { IDD = IDD_ACR122UBASICDEVICEPROGRAMMING_DIALOG };
	CButton	m_ButtonConnect;
	CEdit	m_EditBoxNumberOfRepetitions;
	CButton	m_RadioButtonT2Duration;
	CButton	m_RadioButtonT1Duration;
	CButton	m_RadioButtonT1AndT2Duration;
	CButton	m_RadioButtonLinkToBuzzerOff;
	CButton	m_ButtonSetLedAndBuzzerControl;
	CEdit	m_EditBoxApduLogs;
	CEdit	m_EditBoxT1Duration;
	CEdit	m_EditBoxT2Duration;
	CButton	m_RadioButtonGreenBlinkingMaskOff;
	CButton	m_RadioButtonGreenBlinkingMaskOn;
	CButton	m_RadioButtonGreenFinalStateOff;
	CButton	m_RadioButtonGreenFinalStateOn;
	CButton	m_RadioButtonGreenInitBlinkingStateOff;
	CButton	m_RadioButtonGreenInitBlinkingStateOn;
	CButton	m_RadioButtonGreenStateMaskOff;
	CButton	m_RadioButtonGreenStateMaskOn;
	CButton	m_RadioButtonRedBlinkingMaskOff;
	CButton	m_RadioButtonRedBlinkingMaskOn;
	CButton	m_RadioButtonRedStateMaskOn;
	CButton	m_RadioButtonRedStateMaskOff;
	CButton	m_RadioButtonRedInitBlinkingStateOn;
	CButton	m_RadioButtonRedInitBlinkingStateOff;
	CButton	m_RadioButtonRedFinalOff;
	CButton	m_RadioButtonRedFinalOn;
	CButton	m_ButtonSetBuzzerOutput;
	CButton	m_RadioButtonBuzzerOutputOff;
	CButton	m_RadioButtonBuzzerOutputOn;
	CButton	m_ButtonGetStatus;
	CStatic	m_LabelFirmware;
	CButton	m_ButtonGetFirmware;
	CButton	m_ButtonSetTimeout;
	CEdit	m_EditBoxTimeoutParameter;
	CComboBox	m_ComboBoxReaderList;
	CString	m_EditBoxApduLogsText;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACR122UBasicDeviceProgrammingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CACR122UBasicDeviceProgrammingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void onButtonInitialize();
	afx_msg void onButtonQuit();
	afx_msg void onButtonConnect();
	afx_msg void onButtonFirmware();
	afx_msg void onButtonGetStatus();
	afx_msg void onButtonSetTimeout();
	afx_msg void onButtonSetBuzzerOutput();
	afx_msg void onRadioButtonBuzzerOutputOn();
	afx_msg void onRadioButtonBuzzerOutputOff();
	afx_msg void onButtonSetLedAndBuzzerControl();
	afx_msg void onRadioButtonRedFinalOn();
	afx_msg void onRadioButtonRedFinalOff();
	afx_msg void onRadioButtonRedStateMaskOn();
	afx_msg void onRadioButtonRedStateMaskOff();
	afx_msg void onRadioButtonRedInitBlinkingStateOn();
	afx_msg void onRadioButtonRedInitBlinkingStateOff();
	afx_msg void onRadioButtonRedBlinkingMaskOn();
	afx_msg void onRadioButtonRedBlinkingMaskOff();
	afx_msg void onRadioButtonGreenFinalStateOn();
	afx_msg void onRadioButtonGreenFinalStateOff();
	afx_msg void onRadioButtonGreenStateMaskOn();
	afx_msg void onRadioButtonGreenStateMaskOff();
	afx_msg void onRadioButtonGreenInitBlinkingStateOn();
	afx_msg void onRadioButtonGreenInitBlinkingStateOff();
	afx_msg void onRadioButtonGreenBlinkingMaskOn();
	afx_msg void onRadioButtonGreenBlinkingMaskOff();
	afx_msg void onRadioButtonLinkToBuzzerOff();
	afx_msg void onRadioButtonT2Duration();
	afx_msg void onRadioButtonT1Duration();
	afx_msg void onRadioButtonT1AndT2Duration();
	afx_msg void onButtonClear();
	afx_msg void onButtonReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	static const int STATUS_SIZE;
	static const int STATUS_ERROR_CODE_INDEX;
	static const int STATUS_FIELD_INDEX;
	static const int STATUS_TARGET_NUMBER_INDEX;
	static const int STATUS_LOGICAL_NUMBER_INDEX;
	static const int STATUS_BIT_RATE_RECEPTION_INDEX;
	static const int STATUS_BIT_RATE_TRANSMISSION_INDEX;
	static const int STATUS_MODULATION_TYPE_INDEX;

	static const int STATUS_BYTE_NULL_INDEX;

	enum BIT_RATE
	{
		BIT_RATE_106_KBPS,
		BIT_RATE_212_KBPS,
		BIT_RATE_424_KBPS
	};

	enum MODULATION_TYPE
	{
		MODULATION_TYPE_ISO14443_MIFARE,
		MODULATION_TYPE_ACTIVE_MODE,
		MODULATION_TYPE_FELICA = 0x10,
		MODULATION_TYPE_INNOVATION_JEWEL_TAG
	};

	char aMessage_[100];
	char aStatusByte_[2];

	PcscReader *pPcscReader_;
	Acr122u *pAcr122u_;

	bool checkIfInputIsNotEmpty(CString sInput);
	bool checkIfInputIsInHexFormat(CString sInput);
	bool checkIfInputLengthIsValid(CString sInput,int32 iValidLength);

	void onPcscException(PcscException& cException);
	void onStandardException(const std::exception& cException);
	void onGeneralException();
	void onInitializeException();

	void resetTextsState();
	void resetRadioButtonsState();
	void resetBuzzerOutputRadioButtons();
	void resetRedLedRadioButtons();
	void resetGreenLedRadioButtons();
	void resetLinkToBuzzerRadioButtons();

	void enableFirmwareSettingsControls(bool bIsEnabled);
	void enableTimeOutParametersControls(bool bIsEnabled);
	void enableBuzzerOutputControls(bool bIsEnabled);

	void enableLedAndBuzzerControls(bool bIsEnabled);

	void enableFinalLedStateControls(bool bIsEnabled);
	void enableRedStateMaskControls(bool bIsEnabled);
	void enableInitialBlinkingStateControls(bool bIsEnabled);
	void enableRedLedBlinkingMaskControls(bool bIsEnabled);

	void enableGreenLedControls(bool bIsEnabled);
	void enableGreenFinalLedStateControls(bool bIsEnabled);
	void enableGreenStateMaskControls(bool bIsEnabled);
	void enableGreenInitialBlinkingStateControls(bool bIsEnabled);
	void enableGreenLedBlinkingMaskControls(bool bIsEnabled);

	void enableBlinkingDurationControlControls(bool bIsEnabled);
	void enableLinkToBuzzerControls(bool bIsEnabled);

	void displayListToComboBox(string* pList, int32 iListSize);
	void validateSelectedReaderName(CString sReaderName);

	void validateStatus(int32 iStatusSize);
	string readStatusSummary(byte* pStatus);
	string getBitRateInReception(byte* pStatus);
	string getBitRateInTransmission(byte* pStatus);
	string getModulationType(byte* pStatus);
	string getErrorCode(byte* pStatus);
	string getField(byte* pStatus);
	string getTargetNumber(byte* pStatus);
	string getLogicalNumber(byte* pStatus);

	void validateTimeOutParameterInput(CString sTimeOutParameter);
	void validateT1DurationInput(CString sT1Duration);
	void validateT2DurationInput(CString sT2Duration);
	void validateNumberOfRepetitionInput(CString sNumberOfRepetition);

	void configureLedAndBuzzerControl(LedAndBuzzerControl* pLedAndBuzzerControl);
	void setLedStateControl(LedAndBuzzerControl* pLedAndBuzzerControl);
	void setBlinkingDurationControl(LedAndBuzzerControl* pLedAndBuzzerControl);

	uint32 getT1Duration();
	uint32 getT2Duration();
	uint32 getNumberOfRepetition();
	LINK_TO_BUZZER getLinkToBuzzerDuration();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACR122UBASICDEVICEPROGRAMMINGDLG_H__52ABA0E4_BBA3_4F25_983D_8A31C9CC18EB__INCLUDED_)
