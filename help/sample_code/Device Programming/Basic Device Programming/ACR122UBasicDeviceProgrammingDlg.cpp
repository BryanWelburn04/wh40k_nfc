/*===========================================================================================
 * 
 *  Author          : Julius Ceasar L. Fabunan
 * 
 *  File            : ACR122UBasicDeviceProgramming.cpp
 * 
 *  Copyright (C)   : Advanced Card System Ltd.
 * 
 *  Description     : Contains Methods and Properties for ACR122U SmartCard Reader
 * 
 *  Date            : January 26, 2017
 * 
 *  Revision Traile : [Author] / [Date of modification] / [Details of Modifications done]
 * 
 * 
 * =========================================================================================*/

#include "stdafx.h"
#include "ACR122UBasicDeviceProgramming.h"
#include "ACR122UBasicDeviceProgrammingDlg.h"
#include "Helper.h"
#include "ValidationException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CACR122UBasicDeviceProgrammingDlg *_pThis = NULL;
Helper _cHelper;

const int CACR122UBasicDeviceProgrammingDlg::STATUS_SIZE = 0x08;
const int CACR122UBasicDeviceProgrammingDlg::STATUS_ERROR_CODE_INDEX = 0x02;
const int CACR122UBasicDeviceProgrammingDlg::STATUS_FIELD_INDEX = 0x03;
const int CACR122UBasicDeviceProgrammingDlg::STATUS_TARGET_NUMBER_INDEX = 0x04;
const int CACR122UBasicDeviceProgrammingDlg::STATUS_LOGICAL_NUMBER_INDEX = 0x05;
const int CACR122UBasicDeviceProgrammingDlg::STATUS_BIT_RATE_RECEPTION_INDEX = 0x06;
const int CACR122UBasicDeviceProgrammingDlg::STATUS_BIT_RATE_TRANSMISSION_INDEX = 0x07;
const int CACR122UBasicDeviceProgrammingDlg::STATUS_MODULATION_TYPE_INDEX = 0x08;

const int CACR122UBasicDeviceProgrammingDlg::STATUS_BYTE_NULL_INDEX = 0x01;

void onSendCommand(const byte *pSendData, const int iLength)
{
	_pThis->addMessageToLogs((CString)"<< " + (CString)(_cHelper.byteAsString(pSendData, iLength,true)));
}

void onReceiveCommand(const byte *pReceiveData, const int iLength)
{
	_pThis->addMessageToLogs((CString)">> " + (CString)(_cHelper.byteAsString(pReceiveData, iLength,true)));
}

CACR122UBasicDeviceProgrammingDlg::CACR122UBasicDeviceProgrammingDlg(CWnd* pParent)
	: CDialog(CACR122UBasicDeviceProgrammingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CACR122UBasicDeviceProgrammingDlg)
	m_EditBoxApduLogsText = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_ACSLOGO);
}
CACR122UBasicDeviceProgrammingDlg::~CACR122UBasicDeviceProgrammingDlg()
{
	if(pPcscReader_->isConnectionActive())
		pPcscReader_->disconnect();	

	pPcscReader_->cleanup();

	delete pPcscReader_;
	delete pAcr122u_;
}

void CACR122UBasicDeviceProgrammingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CACR122UBasicDeviceProgrammingDlg)
	DDX_Control(pDX, IDC_ButtonConnect, m_ButtonConnect);
	DDX_Control(pDX, IDC_EditBoxNumberOfRepetitions, m_EditBoxNumberOfRepetitions);
	DDX_Control(pDX, IDC_RadioButtonT2Duration, m_RadioButtonT2Duration);
	DDX_Control(pDX, IDC_RadioButtonT1Duration, m_RadioButtonT1Duration);
	DDX_Control(pDX, IDC_RadioButtonT1AndT2Duration, m_RadioButtonT1AndT2Duration);
	DDX_Control(pDX, IDC_RadioButtonLinkToBuzzerOff, m_RadioButtonLinkToBuzzerOff);
	DDX_Control(pDX, IDC_ButtonSetLedAndBuzzerControl, m_ButtonSetLedAndBuzzerControl);
	DDX_Control(pDX, IDC_EditBoxApduLogs, m_EditBoxApduLogs);
	DDX_Control(pDX, IDC_EditBoxT1Duration, m_EditBoxT1Duration);
	DDX_Control(pDX, IDC_EditBoxT2Duration, m_EditBoxT2Duration);
	DDX_Control(pDX, IDC_RadioButtonGreenBlinkingMaskOff, m_RadioButtonGreenBlinkingMaskOff);
	DDX_Control(pDX, IDC_RadioButtonGreenBlinkingMaskOn, m_RadioButtonGreenBlinkingMaskOn);
	DDX_Control(pDX, IDC_RadioButtonGreenFinalStateOff, m_RadioButtonGreenFinalStateOff);
	DDX_Control(pDX, IDC_RadioButtonGreenFinalStateOn, m_RadioButtonGreenFinalStateOn);
	DDX_Control(pDX, IDC_RadioButtonGreenInitBlinkingStateOff, m_RadioButtonGreenInitBlinkingStateOff);
	DDX_Control(pDX, IDC_RadioButtonGreenInitBlinkingStateOn, m_RadioButtonGreenInitBlinkingStateOn);
	DDX_Control(pDX, IDC_RadioButtonGreenStateMaskOff, m_RadioButtonGreenStateMaskOff);
	DDX_Control(pDX, IDC_RadioButtonGreenStateMaskOn, m_RadioButtonGreenStateMaskOn);
	DDX_Control(pDX, IDC_RadioButtonRedBlinkingMaskOff, m_RadioButtonRedBlinkingMaskOff);
	DDX_Control(pDX, IDC_RadioButtonRedBlinkingMaskOn, m_RadioButtonRedBlinkingMaskOn);
	DDX_Control(pDX, IDC_RadioButtonRedStateMaskOn, m_RadioButtonRedStateMaskOn);
	DDX_Control(pDX, IDC_RadioButtonRedStateMaskOff, m_RadioButtonRedStateMaskOff);
	DDX_Control(pDX, IDC_RadioButtonRedInitBlinkingStateOn, m_RadioButtonRedInitBlinkingStateOn);
	DDX_Control(pDX, IDC_RadioButtonRedInitBlinkingStateOff, m_RadioButtonRedInitBlinkingStateOff);
	DDX_Control(pDX, IDC_RadioButtonRedFinalOff, m_RadioButtonRedFinalOff);
	DDX_Control(pDX, IDC_RadioButtonRedFinalOn, m_RadioButtonRedFinalOn);
	DDX_Control(pDX, IDC_ButtonSetBuzzerOutput, m_ButtonSetBuzzerOutput);
	DDX_Control(pDX, IDC_RadioButtonBuzzerOutputOff, m_RadioButtonBuzzerOutputOff);
	DDX_Control(pDX, IDC_RadioButtonBuzzerOutputOn, m_RadioButtonBuzzerOutputOn);
	DDX_Control(pDX, IDC_ButtonGetStatus, m_ButtonGetStatus);
	DDX_Control(pDX, IDC_LabelFirmware, m_LabelFirmware);
	DDX_Control(pDX, IDC_ButtonGetFirmware, m_ButtonGetFirmware);
	DDX_Control(pDX, IDC_ButtonSetTimeout, m_ButtonSetTimeout);
	DDX_Control(pDX, IDC_EditBoxTimeoutParameter, m_EditBoxTimeoutParameter);
	DDX_Control(pDX, IDC_ComboBoxReaderList, m_ComboBoxReaderList);
	DDX_Text(pDX, IDC_EditBoxApduLogs, m_EditBoxApduLogsText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CACR122UBasicDeviceProgrammingDlg, CDialog)
	//{{AFX_MSG_MAP(CACR122UBasicDeviceProgrammingDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ButtonInitialize, onButtonInitialize)
	ON_BN_CLICKED(IDC_ButtonQuit, onButtonQuit)
	ON_BN_CLICKED(IDC_ButtonConnect, onButtonConnect)
	ON_BN_CLICKED(IDC_ButtonGetFirmware, onButtonFirmware)
	ON_BN_CLICKED(IDC_ButtonGetStatus, onButtonGetStatus)
	ON_BN_CLICKED(IDC_ButtonSetTimeout, onButtonSetTimeout)
	ON_BN_CLICKED(IDC_ButtonSetBuzzerOutput, onButtonSetBuzzerOutput)
	ON_BN_CLICKED(IDC_RadioButtonBuzzerOutputOn, onRadioButtonBuzzerOutputOn)
	ON_BN_CLICKED(IDC_RadioButtonBuzzerOutputOff, onRadioButtonBuzzerOutputOff)
	ON_BN_CLICKED(IDC_ButtonSetLedAndBuzzerControl, onButtonSetLedAndBuzzerControl)
	ON_BN_CLICKED(IDC_RadioButtonRedFinalOn, onRadioButtonRedFinalOn)
	ON_BN_CLICKED(IDC_RadioButtonRedFinalOff, onRadioButtonRedFinalOff)
	ON_BN_CLICKED(IDC_RadioButtonRedStateMaskOn, onRadioButtonRedStateMaskOn)
	ON_BN_CLICKED(IDC_RadioButtonRedStateMaskOff, onRadioButtonRedStateMaskOff)
	ON_BN_CLICKED(IDC_RadioButtonRedInitBlinkingStateOn, onRadioButtonRedInitBlinkingStateOn)
	ON_BN_CLICKED(IDC_RadioButtonRedInitBlinkingStateOff, onRadioButtonRedInitBlinkingStateOff)
	ON_BN_CLICKED(IDC_RadioButtonRedBlinkingMaskOn, onRadioButtonRedBlinkingMaskOn)
	ON_BN_CLICKED(IDC_RadioButtonRedBlinkingMaskOff, onRadioButtonRedBlinkingMaskOff)
	ON_BN_CLICKED(IDC_RadioButtonGreenFinalStateOn, onRadioButtonGreenFinalStateOn)
	ON_BN_CLICKED(IDC_RadioButtonGreenFinalStateOff, onRadioButtonGreenFinalStateOff)
	ON_BN_CLICKED(IDC_RadioButtonGreenStateMaskOn, onRadioButtonGreenStateMaskOn)
	ON_BN_CLICKED(IDC_RadioButtonGreenStateMaskOff, onRadioButtonGreenStateMaskOff)
	ON_BN_CLICKED(IDC_RadioButtonGreenInitBlinkingStateOn, onRadioButtonGreenInitBlinkingStateOn)
	ON_BN_CLICKED(IDC_RadioButtonGreenInitBlinkingStateOff, onRadioButtonGreenInitBlinkingStateOff)
	ON_BN_CLICKED(IDC_RadioButtonGreenBlinkingMaskOn, onRadioButtonGreenBlinkingMaskOn)
	ON_BN_CLICKED(IDC_RadioButtonGreenBlinkingMaskOff, onRadioButtonGreenBlinkingMaskOff)
	ON_BN_CLICKED(IDC_RadioButtonLinkToBuzzerOff, onRadioButtonLinkToBuzzerOff)
	ON_BN_CLICKED(IDC_RadioButtonT2Duration, onRadioButtonT2Duration)
	ON_BN_CLICKED(IDC_RadioButtonT1Duration, onRadioButtonT1Duration)
	ON_BN_CLICKED(IDC_RadioButtonT1AndT2Duration, onRadioButtonT1AndT2Duration)
	ON_BN_CLICKED(IDC_ButtonClear, onButtonClear)
	ON_BN_CLICKED(IDC_ButtonReset, onButtonReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CACR122UBasicDeviceProgrammingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_pThis = this;

	aStatusByte_[STATUS_BYTE_NULL_INDEX] = '\0';

	pPcscReader_ = new PcscReader();
	pAcr122u_ = new Acr122u(pPcscReader_);

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	
	resetControls();
	
	return TRUE;
}

void CACR122UBasicDeviceProgrammingDlg::OnPaint() 
{
	CPaintDC cDc(this);
	CRect cRect;

	int iCxIcon, iCyIcon;
	int iX, iY;

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, (WPARAM) cDc.GetSafeHdc(), 0);

		iCxIcon = GetSystemMetrics(SM_CXICON);
		iCyIcon = GetSystemMetrics(SM_CYICON);
		
		GetClientRect(&cRect);
		iX = (cRect.Width() - iCxIcon + 1) / 2;
		iY = (cRect.Height() - iCyIcon + 1) / 2;

		cDc.DrawIcon(iX, iY, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CACR122UBasicDeviceProgrammingDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CACR122UBasicDeviceProgrammingDlg::onPcscException(PcscException& cException)
{
	string sLogMessage;

	sprintf(aMessage_,"[%ld] %s",cException.getErrorCode(),cException.getMessage());
	sLogMessage = "\r\n" + string(aMessage_);
	addMessageToLogs(sLogMessage.c_str());
	MessageBox(aMessage_, "Error", MB_ICONERROR);
}
void CACR122UBasicDeviceProgrammingDlg::onStandardException(const std::exception& cException)
{
	MessageBox(cException.what(), "Error", MB_ICONERROR);
}
void CACR122UBasicDeviceProgrammingDlg::onGeneralException()
{	
	addMessageToLogs("\r\nGeneral exception occured.");
	MessageBox("\r\nGeneral exception occured.", "Error", MB_ICONERROR);
}

void CACR122UBasicDeviceProgrammingDlg::resetControls()
{
	enableControls(false);
	resetRadioButtonsState();
	resetTextsState();
	m_ButtonConnect.EnableWindow(false);
	m_ComboBoxReaderList.ResetContent();
	addMessageToLogs("Program Ready");
}

void CACR122UBasicDeviceProgrammingDlg::enableControls(bool bIsEnabled)
{
	enableFirmwareSettingsControls(bIsEnabled);
	enableTimeOutParametersControls(bIsEnabled);
	enableBuzzerOutputControls(bIsEnabled);

	enableLedAndBuzzerControls(bIsEnabled);

	enableFinalLedStateControls(bIsEnabled);
	enableRedStateMaskControls(bIsEnabled);
	enableInitialBlinkingStateControls(bIsEnabled);
	enableRedLedBlinkingMaskControls(bIsEnabled);

	enableGreenLedControls(bIsEnabled);
	enableGreenFinalLedStateControls(bIsEnabled);
	enableGreenStateMaskControls(bIsEnabled);
	enableGreenInitialBlinkingStateControls(bIsEnabled);
	enableGreenLedBlinkingMaskControls(bIsEnabled);

	enableBlinkingDurationControlControls(bIsEnabled);
	enableLinkToBuzzerControls(bIsEnabled);	
}

void CACR122UBasicDeviceProgrammingDlg::resetRadioButtonsState()
{
	resetBuzzerOutputRadioButtons();
	resetRedLedRadioButtons();
	resetGreenLedRadioButtons();
	resetLinkToBuzzerRadioButtons();
}
void CACR122UBasicDeviceProgrammingDlg::resetTextsState()
{
	m_LabelFirmware.SetWindowText("");
	m_EditBoxNumberOfRepetitions.SetWindowText("");
	m_EditBoxT1Duration.SetWindowText("");
	m_EditBoxT2Duration.SetWindowText("");
	m_EditBoxTimeoutParameter.SetWindowText("");
	m_EditBoxApduLogs.SetWindowText("");
}

void CACR122UBasicDeviceProgrammingDlg::enableFirmwareSettingsControls(bool bIsEnabled)
{
	GetDlgItem(IDC_GroupBoxFirmware)->EnableWindow(bIsEnabled);
	m_LabelFirmware.EnableWindow(bIsEnabled);
	m_ButtonGetFirmware.EnableWindow(bIsEnabled);
	m_ButtonGetStatus.EnableWindow(bIsEnabled);
}

void CACR122UBasicDeviceProgrammingDlg::enableTimeOutParametersControls(bool bIsEnabled)
{
	GetDlgItem(IDC_GroupBoxTimeoutParameter)->EnableWindow(bIsEnabled);
	GetDlgItem(IDC_LabelTimeoutParameter)->EnableWindow(bIsEnabled);
	m_EditBoxTimeoutParameter.EnableWindow(bIsEnabled);
	m_ButtonSetTimeout.EnableWindow(bIsEnabled);
}

void CACR122UBasicDeviceProgrammingDlg::enableBuzzerOutputControls(bool bIsEnabled)
{
	GetDlgItem(IDC_GroupBoxBuzzerOutput)->EnableWindow(bIsEnabled);
	m_RadioButtonBuzzerOutputOn.EnableWindow(bIsEnabled);
	m_RadioButtonBuzzerOutputOff.EnableWindow(bIsEnabled);
	m_ButtonSetBuzzerOutput.EnableWindow(bIsEnabled);
}

void CACR122UBasicDeviceProgrammingDlg::enableLedAndBuzzerControls(bool bIsEnabled)
{
	GetDlgItem(IDC_GroupBoxLedAndBuzzerControl)->EnableWindow(bIsEnabled);
	GetDlgItem(IDC_GroupBoxLedStateControl)->EnableWindow(bIsEnabled);
	GetDlgItem(IDC_GroupBoxRedLed)->EnableWindow(bIsEnabled);
}

void CACR122UBasicDeviceProgrammingDlg::enableFinalLedStateControls(bool bIsEnabled)
{
	GetDlgItem(IDC_GroupBoxFinalLedState)->EnableWindow(bIsEnabled);
	m_RadioButtonRedFinalOn.EnableWindow(bIsEnabled);
	m_RadioButtonRedFinalOff.EnableWindow(bIsEnabled);
}

void CACR122UBasicDeviceProgrammingDlg::enableRedStateMaskControls(bool bIsEnabled)
{
	GetDlgItem(IDC_GroupBoxRedStateMask)->EnableWindow(bIsEnabled);
	m_RadioButtonRedStateMaskOn.EnableWindow(bIsEnabled);
	m_RadioButtonRedStateMaskOff.EnableWindow(bIsEnabled);
}

void CACR122UBasicDeviceProgrammingDlg::enableInitialBlinkingStateControls(bool bIsEnabled)
{
	GetDlgItem(IDC_GroupBoxInitialBlinkingState)->EnableWindow(bIsEnabled);
	m_RadioButtonRedInitBlinkingStateOn.EnableWindow(bIsEnabled);
	m_RadioButtonRedInitBlinkingStateOff.EnableWindow(bIsEnabled);
}

void CACR122UBasicDeviceProgrammingDlg::enableRedLedBlinkingMaskControls(bool bIsEnabled)
{
	GetDlgItem(IDC_GroupBoxRedLedBlinkingMask)->EnableWindow(bIsEnabled);
	m_RadioButtonRedBlinkingMaskOn.EnableWindow(bIsEnabled);
	m_RadioButtonRedBlinkingMaskOff.EnableWindow(bIsEnabled);
}

void CACR122UBasicDeviceProgrammingDlg::enableGreenLedControls(bool bIsEnabled)
{
	GetDlgItem(IDC_GroupBoxGreenLed)->EnableWindow(bIsEnabled);
}

void CACR122UBasicDeviceProgrammingDlg::enableGreenFinalLedStateControls(bool bIsEnabled)
{
	GetDlgItem(IDC_GroupBoxGreenFinalLedState)->EnableWindow(bIsEnabled);
	m_RadioButtonGreenFinalStateOn.EnableWindow(bIsEnabled);
	m_RadioButtonGreenFinalStateOff.EnableWindow(bIsEnabled);
}

void CACR122UBasicDeviceProgrammingDlg::enableGreenStateMaskControls(bool bIsEnabled)
{
	GetDlgItem(IDC_GroupBoxGreenStateMask)->EnableWindow(bIsEnabled);
	m_RadioButtonGreenStateMaskOn.EnableWindow(bIsEnabled);
	m_RadioButtonGreenStateMaskOff.EnableWindow(bIsEnabled);
}

void CACR122UBasicDeviceProgrammingDlg::enableGreenInitialBlinkingStateControls(bool bIsEnabled)
{
	GetDlgItem(IDC_GroupBoxGreenInitialBlinkingState)->EnableWindow(bIsEnabled);
	m_RadioButtonGreenInitBlinkingStateOn.EnableWindow(bIsEnabled);
	m_RadioButtonGreenInitBlinkingStateOff.EnableWindow(bIsEnabled);
}

void CACR122UBasicDeviceProgrammingDlg::enableGreenLedBlinkingMaskControls(bool bIsEnabled)
{
	GetDlgItem(IDC_GroupBoxGreenLedBlinkingMask)->EnableWindow(bIsEnabled);
	m_RadioButtonGreenBlinkingMaskOn.EnableWindow(bIsEnabled);
	m_RadioButtonGreenBlinkingMaskOff.EnableWindow(bIsEnabled);
}

void CACR122UBasicDeviceProgrammingDlg::enableBlinkingDurationControlControls(bool bIsEnabled)
{
	GetDlgItem(IDC_GroupBoxBlinkingDurationControl)->EnableWindow(bIsEnabled);
	GetDlgItem(IDC_Labelx100ms)->EnableWindow(bIsEnabled);
	GetDlgItem(IDC_Labelx100ms2)->EnableWindow(bIsEnabled);
	GetDlgItem(IDC_LabelT1DurationInitial)->EnableWindow(bIsEnabled);
	GetDlgItem(IDC_LabelBlinkingState)->EnableWindow(bIsEnabled);
	GetDlgItem(IDC_LabelT2DurationToggle)->EnableWindow(bIsEnabled);
	GetDlgItem(IDC_LabelBlinkingState2)->EnableWindow(bIsEnabled);
	GetDlgItem(IDC_LabelNumberOfRepetition)->EnableWindow(bIsEnabled);
	m_EditBoxT1Duration.EnableWindow(bIsEnabled);
	m_EditBoxT2Duration.EnableWindow(bIsEnabled);
	m_EditBoxNumberOfRepetitions.EnableWindow(bIsEnabled);
}

void CACR122UBasicDeviceProgrammingDlg::enableLinkToBuzzerControls(bool bIsEnabled)
{
	GetDlgItem(IDC_GroupBoxLinkToBuzzer)->EnableWindow(bIsEnabled);
	m_RadioButtonLinkToBuzzerOff.EnableWindow(bIsEnabled);
	m_RadioButtonT2Duration.EnableWindow(bIsEnabled);
	m_RadioButtonT1Duration.EnableWindow(bIsEnabled);
	m_RadioButtonT1AndT2Duration.EnableWindow(bIsEnabled);
	m_ButtonSetLedAndBuzzerControl.EnableWindow(bIsEnabled);
}

void CACR122UBasicDeviceProgrammingDlg::resetBuzzerOutputRadioButtons()
{
	m_RadioButtonBuzzerOutputOn.SetCheck(true);
	m_RadioButtonBuzzerOutputOff.SetCheck(false);
}

void CACR122UBasicDeviceProgrammingDlg::resetRedLedRadioButtons()
{
	m_RadioButtonRedBlinkingMaskOn.SetCheck(true);
	m_RadioButtonRedBlinkingMaskOff.SetCheck(false);
	m_RadioButtonRedFinalOn.SetCheck(true);
	m_RadioButtonRedFinalOff.SetCheck(false);
	m_RadioButtonRedInitBlinkingStateOn.SetCheck(true);
	m_RadioButtonRedInitBlinkingStateOff.SetCheck(false);
	m_RadioButtonRedStateMaskOn.SetCheck(true);
	m_RadioButtonRedStateMaskOff.SetCheck(false);
}

void CACR122UBasicDeviceProgrammingDlg::resetGreenLedRadioButtons()
{
	m_RadioButtonGreenBlinkingMaskOn.SetCheck(true);
	m_RadioButtonGreenBlinkingMaskOff.SetCheck(false);
	m_RadioButtonGreenFinalStateOn.SetCheck(true);
	m_RadioButtonGreenFinalStateOff.SetCheck(false);
	m_RadioButtonGreenInitBlinkingStateOn.SetCheck(true);
	m_RadioButtonGreenInitBlinkingStateOff.SetCheck(false);
	m_RadioButtonGreenStateMaskOn.SetCheck(true);
	m_RadioButtonGreenStateMaskOff.SetCheck(false);
}

void CACR122UBasicDeviceProgrammingDlg::resetLinkToBuzzerRadioButtons()
{
	m_RadioButtonLinkToBuzzerOff.SetCheck(true);
	m_RadioButtonT2Duration.SetCheck(false);
	m_RadioButtonT1Duration.SetCheck(false);
	m_RadioButtonT1AndT2Duration.SetCheck(false);
}

void CACR122UBasicDeviceProgrammingDlg::addMessageToLogs(CString sMessage)
{
	UpdateData(true);
	sMessage = sMessage + "\r\n";
	m_EditBoxApduLogsText += sMessage;
	UpdateData(false);

	m_EditBoxApduLogs.SetSel(0xffff, 0xffff);
}

bool CACR122UBasicDeviceProgrammingDlg::checkIfInputIsNotEmpty(CString sInput)
{
	if(strcmp(sInput, "") == 0)
		return false;

	return true;
}
bool CACR122UBasicDeviceProgrammingDlg::checkIfInputIsInHexFormat(CString sInput)
{
	int32 iCharIterator;

	for(iCharIterator = 0; iCharIterator < strlen(sInput); ++iCharIterator)
	{
		if(!_cHelper.checkIfValidHexCharacter(sInput[iCharIterator]))
			return false;
	}

	return true;
}
bool CACR122UBasicDeviceProgrammingDlg::checkIfInputLengthIsValid(CString sInput,int32 iValidLength)
{
	if(strlen(sInput) != iValidLength)
		return false;

	return true;
}

void CACR122UBasicDeviceProgrammingDlg::onButtonInitialize() 
{
	string *pReaderList = new string[10];
	uint8 uReaderCount = 10;

	try
	{	
		m_ComboBoxReaderList.ResetContent();

		if(pPcscReader_->isConnectionActive())
			pPcscReader_->disconnect();	

		pPcscReader_->cleanup();
		pPcscReader_->initialize();
		pPcscReader_->listReaders(pReaderList, uReaderCount);
		pPcscReader_->setSendCommandFunctor(onSendCommand);
		pPcscReader_->setReceiveCommandFunctor(onReceiveCommand);
		displayListToComboBox(pReaderList,uReaderCount);
		addMessageToLogs("\r\nInitialize success");
		enableControls(false);
		m_ComboBoxReaderList.SetCurSel(0);
		m_ButtonConnect.EnableWindow(true);
	}
	catch(PcscException cException){ onInitializeException(); onPcscException(cException); }
	catch(const std::exception &cException) { onInitializeException(); onStandardException(cException); }
	catch(...) { onInitializeException(); onGeneralException(); }

	delete[] pReaderList;
}

void CACR122UBasicDeviceProgrammingDlg::onInitializeException()
{
	enableControls(false);
	m_ButtonConnect.EnableWindow(false);
}

void CACR122UBasicDeviceProgrammingDlg::displayListToComboBox(string* pList, int32 iListSize)
{
	int32 iIterator;

	for(iIterator = 0; iIterator < iListSize; ++iIterator)
	{
		m_ComboBoxReaderList.AddString(pList[iIterator].data());
	}
}

void CACR122UBasicDeviceProgrammingDlg::onButtonConnect() 
{
	CString sReaderName;

	try
	{
		m_ComboBoxReaderList.GetWindowText(sReaderName);
		validateSelectedReaderName(sReaderName);
		pPcscReader_->connectDirect((CString)sReaderName);
		sprintf(aMessage_,"\r\n%s %s", "Successfully connected to ", sReaderName);
		addMessageToLogs(aMessage_);
		enableControls(true);
        m_ButtonConnect.EnableWindow(false);
	}
	catch(PcscException cException){ onPcscException(cException); }
	catch(const std::exception &cException) { onStandardException(cException); }
	catch(...) { onGeneralException(); }
}

void CACR122UBasicDeviceProgrammingDlg::validateSelectedReaderName(CString sReaderName)
{
	if (!strcmp(sReaderName, ""))
	{
		throw ValidationException("Please select smartcard reader");
	}
}

void CACR122UBasicDeviceProgrammingDlg::onButtonFirmware() 
{
	CString sFirmwareVersion;

	try
	{
		addMessageToLogs("\r\nGet Firmware Version");
		sFirmwareVersion = pAcr122u_->getFirmwareVersion();
		m_LabelFirmware.SetWindowText(sFirmwareVersion);
	}
	catch(PcscException cException) { onPcscException(cException); }
	catch(const std::exception &cException) { onStandardException(cException); }
	catch(...) { onGeneralException(); }
}

void CACR122UBasicDeviceProgrammingDlg::onButtonGetStatus() 
{
	byte* pStatus;
	string sStringStatus = "";
	int32 iStatusSize = 0;

	try
	{
		addMessageToLogs("\r\nGet Status");
		pStatus = pAcr122u_->getStatus(iStatusSize);
		validateStatus(iStatusSize);
		sStringStatus = readStatusSummary(pStatus);
		addMessageToLogs(sStringStatus.c_str());
		
		delete pStatus;	
	}
	catch(PcscException cException) { onPcscException(cException); }
	catch(ValidationException cException) { addMessageToLogs(cException.what()); }
	catch(const std::exception &cException) { onStandardException(cException); }
	catch(...) { onGeneralException(); }
}

void CACR122UBasicDeviceProgrammingDlg::validateStatus(int32 iStatusSize)
{
	if(iStatusSize <= STATUS_SIZE)
		throw ValidationException("No tag is in the field");
}

string CACR122UBasicDeviceProgrammingDlg::readStatusSummary(byte* pStatus)
{
	string sStringStatus = "";

	sStringStatus = "\r\nError Code: " + getErrorCode(pStatus);
	sStringStatus += "\r\nField: " + getField(pStatus);
	sStringStatus += "\r\nNo. of Targets: " + getTargetNumber(pStatus);
	sStringStatus += "\r\nLogical No.: " + getLogicalNumber(pStatus);
	sStringStatus += "\r\nBit Rate in Reception: " + getBitRateInReception(pStatus);
	sStringStatus += "\r\nBit Rate in Transmission: " + getBitRateInTransmission(pStatus);
	sStringStatus += "\r\nModulation Type: " + getModulationType(pStatus);

	return sStringStatus;
}

string CACR122UBasicDeviceProgrammingDlg::getErrorCode(byte* pStatus)
{
	sprintf(aStatusByte_,"%02X",pStatus[STATUS_ERROR_CODE_INDEX]);
	return string(aStatusByte_);
}

string CACR122UBasicDeviceProgrammingDlg::getField(byte* pStatus)
{
	sprintf(aStatusByte_,"%02X",pStatus[STATUS_FIELD_INDEX]);
	return string(aStatusByte_);
}

string CACR122UBasicDeviceProgrammingDlg::getTargetNumber(byte* pStatus)
{
	sprintf(aStatusByte_,"%02X",pStatus[STATUS_TARGET_NUMBER_INDEX]);
	return string(aStatusByte_);
}

string CACR122UBasicDeviceProgrammingDlg::getLogicalNumber(byte* pStatus)
{
	sprintf(aStatusByte_,"%02X",pStatus[STATUS_LOGICAL_NUMBER_INDEX]);
	return string(aStatusByte_);
}

string CACR122UBasicDeviceProgrammingDlg::getBitRateInReception(byte* pStatus)
{
	string sBitRateInReception;

	if (pStatus[STATUS_BIT_RATE_RECEPTION_INDEX] == static_cast<int>(BIT_RATE_106_KBPS))
		sBitRateInReception = "106 Kbps";
	else if (pStatus[STATUS_BIT_RATE_RECEPTION_INDEX] == static_cast<int>(BIT_RATE_212_KBPS))
		sBitRateInReception = "212 Kbps";
	else
		sBitRateInReception = "424 Kbps";

	return sBitRateInReception;
}
string CACR122UBasicDeviceProgrammingDlg::getBitRateInTransmission(byte* pStatus)
{
	string sBitRateInTransmission;

	if (pStatus[STATUS_BIT_RATE_TRANSMISSION_INDEX] == static_cast<int>(BIT_RATE_106_KBPS))
		sBitRateInTransmission = "106 Kbps";
	else if (pStatus[STATUS_BIT_RATE_TRANSMISSION_INDEX] == static_cast<int>(BIT_RATE_212_KBPS))
		sBitRateInTransmission = "212 Kbps";
	else
		sBitRateInTransmission = "424 Kbps";

	return sBitRateInTransmission;
}
string CACR122UBasicDeviceProgrammingDlg::getModulationType(byte* pStatus)
{
	string sModulationType;

	if (pStatus[STATUS_MODULATION_TYPE_INDEX] == static_cast<int>(MODULATION_TYPE_ISO14443_MIFARE))
		sModulationType = "ISO 14443 or Mifare";
	else if (pStatus[STATUS_MODULATION_TYPE_INDEX] == static_cast<int>(MODULATION_TYPE_FELICA))
		sModulationType = "Felica";
	else if (pStatus[STATUS_MODULATION_TYPE_INDEX] == static_cast<int>(MODULATION_TYPE_ACTIVE_MODE))
		sModulationType = "Active mode";
	else
		sModulationType = "Innovation Jewel Tag";

	return sModulationType;
}

void CACR122UBasicDeviceProgrammingDlg::onButtonSetTimeout() 
{
	CString sTimeOutParameter;
	int32 iTimeOutParameter;
	byte uTimeOutParameter = 0x00;

    try 
    {
		m_EditBoxTimeoutParameter.GetWindowText(sTimeOutParameter);
		validateTimeOutParameterInput(sTimeOutParameter);
		iTimeOutParameter = strtol(sTimeOutParameter,NULL,16);
		uTimeOutParameter = iTimeOutParameter;
		addMessageToLogs("\r\nSet Timeout Parameter");
		pAcr122u_->setTimeOutParameter(uTimeOutParameter);
    }
	catch(PcscException cException){ onPcscException(cException); }
	catch(const std::exception &cException) { m_EditBoxTimeoutParameter.SetFocus(); onStandardException(cException); }
	catch(...) { onGeneralException(); }
}

void CACR122UBasicDeviceProgrammingDlg::validateTimeOutParameterInput(CString sTimeOutParameter)
{
	static const int VALID_INPUT_LENGTH = 0x02;

	if(!checkIfInputIsNotEmpty(sTimeOutParameter))
		throw ValidationException("Please key-in hex value for Timeout Parameter.");
	
	if(!checkIfInputLengthIsValid(sTimeOutParameter,VALID_INPUT_LENGTH))
		throw ValidationException("Invalid input length. Length must be 2.");

	if(!checkIfInputIsInHexFormat(sTimeOutParameter))
		throw ValidationException("Please key-in hex value for Timeout Parameter.");
}

void CACR122UBasicDeviceProgrammingDlg::onButtonSetBuzzerOutput() 
{
	BUZZER_OUTPUT eBuzzerOuput = BUZZER_OUTPUT_OFF;

    try
    {
        if (m_RadioButtonBuzzerOutputOn.GetCheck() == 1)
            eBuzzerOuput = BUZZER_OUTPUT_ON;

        addMessageToLogs("\r\nSet Buzzer Output");
        pAcr122u_->setBuzzerOutput((byte)eBuzzerOuput);
    }
    catch(PcscException cException) { onPcscException(cException); }
	catch(const std::exception &cException) { onStandardException(cException); }
	catch(...) { onGeneralException(); }
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonBuzzerOutputOn() 
{
	m_RadioButtonBuzzerOutputOff.SetCheck(false);
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonBuzzerOutputOff() 
{
	m_RadioButtonBuzzerOutputOn.SetCheck(false);
}

void CACR122UBasicDeviceProgrammingDlg::onButtonSetLedAndBuzzerControl() 
{
	LedAndBuzzerControl *pLedAndBuzzerControl;

    try
    {
        pLedAndBuzzerControl = new LedAndBuzzerControl();
		configureLedAndBuzzerControl(pLedAndBuzzerControl);
        addMessageToLogs("\r\nSet LED and Buzzer Control");
		delete pAcr122u_->setLedStatus(pLedAndBuzzerControl);
    }
	catch(PcscException cException) { onPcscException(cException); }
	catch(const std::exception &cException) { onStandardException(cException); }
	catch(...){ onGeneralException(); }

	delete pLedAndBuzzerControl;
}

void CACR122UBasicDeviceProgrammingDlg::configureLedAndBuzzerControl(LedAndBuzzerControl* pLedAndBuzzerControl)
{
	setLedStateControl(pLedAndBuzzerControl);
	setBlinkingDurationControl(pLedAndBuzzerControl);
}

void CACR122UBasicDeviceProgrammingDlg::setLedStateControl(LedAndBuzzerControl* pLedAndBuzzerControl)
{
	if(m_RadioButtonRedFinalOn.GetCheck() == 1)
		pLedAndBuzzerControl->setRedLedState(LED_STATE_ON);

    if (m_RadioButtonRedStateMaskOn.GetCheck() == 1)
        pLedAndBuzzerControl->setRedLedStateMask(LED_STATE_MASK_ON);

    if (m_RadioButtonRedInitBlinkingStateOn.GetCheck() == 1)
        pLedAndBuzzerControl->setRedLedBlinkingState(INITIAL_LED_BLINKING_STATE_ON);

    if (m_RadioButtonRedBlinkingMaskOn.GetCheck() == 1)
        pLedAndBuzzerControl->setRedLedBlinkingMask(LED_BLINKING_MASK_ON);

    if (m_RadioButtonGreenFinalStateOn.GetCheck() == 1)
        pLedAndBuzzerControl->setGreenLedState(LED_STATE_ON);

    if (m_RadioButtonGreenStateMaskOn.GetCheck() == 1)
        pLedAndBuzzerControl->setGreenLedStateMask(LED_STATE_MASK_ON);

    if (m_RadioButtonGreenInitBlinkingStateOn.GetCheck() == 1)
        pLedAndBuzzerControl->setGreenLedBlinkingState(INITIAL_LED_BLINKING_STATE_ON);

    if (m_RadioButtonGreenBlinkingMaskOn.GetCheck() == 1)
        pLedAndBuzzerControl->setGreenLedBlinkingMask(LED_BLINKING_MASK_ON);
}
void CACR122UBasicDeviceProgrammingDlg::setBlinkingDurationControl(LedAndBuzzerControl* pLedAndBuzzerControl)
{
	byte aBlinkingDuration[4];

	aBlinkingDuration[0] = getT1Duration();
	aBlinkingDuration[1] = getT2Duration();
	aBlinkingDuration[2] = getNumberOfRepetition();
	aBlinkingDuration[3] = (byte)getLinkToBuzzerDuration();

	pLedAndBuzzerControl->setBlinkingDuration(aBlinkingDuration);
}
uint32 CACR122UBasicDeviceProgrammingDlg::getT1Duration()
{
	CString sT1Duration;

	m_EditBoxT1Duration.GetWindowText(sT1Duration);
	validateT1DurationInput(sT1Duration);
	return strtol(sT1Duration,NULL,16);
}
uint32 CACR122UBasicDeviceProgrammingDlg::getT2Duration()
{
	CString sT2Duration;

	m_EditBoxT2Duration.GetWindowText(sT2Duration);
	validateT2DurationInput(sT2Duration);
	return strtol(sT2Duration,NULL,16);
}
uint32 CACR122UBasicDeviceProgrammingDlg::getNumberOfRepetition()
{
	CString sNumberOfRepetitions;

	m_EditBoxNumberOfRepetitions.GetWindowText(sNumberOfRepetitions);
	validateNumberOfRepetitionInput(sNumberOfRepetitions);
	return strtol(sNumberOfRepetitions,NULL,16);
}

LINK_TO_BUZZER CACR122UBasicDeviceProgrammingDlg::getLinkToBuzzerDuration()
{
	if (m_RadioButtonT1Duration.GetCheck() == 1)
        return LINK_TO_BUZZER_T1_DURATION;
    else if (m_RadioButtonT2Duration.GetCheck() == 1)
        return LINK_TO_BUZZER_T2_DURATION;
    else if (m_RadioButtonT1AndT2Duration.GetCheck() == 1)
        return LINK_TO_BUZZER_T1_AND_T2_DURATION;
	else
		return LINK_TO_BUZZER_OFF;
}

void CACR122UBasicDeviceProgrammingDlg::validateT1DurationInput(CString sT1Duration)
{
	static const int VALID_INPUT_LENGTH = 0x02;

	if(!checkIfInputIsNotEmpty(sT1Duration))
	{
		m_EditBoxT1Duration.SetFocus();
		throw ValidationException("Please key-in hex value for T1 Duration Initial Blinking State.");
	}
	
	if(!checkIfInputLengthIsValid(sT1Duration,VALID_INPUT_LENGTH))
	{
		m_EditBoxT1Duration.SetFocus();
		throw ValidationException("Invalid input length. Length must be 2.");
	}

	if(!checkIfInputIsInHexFormat(sT1Duration))
	{
		m_EditBoxT1Duration.SetFocus();
		throw ValidationException("Please key-in hex value for T1 Duration Initial Blinking State.");
	}
}

void CACR122UBasicDeviceProgrammingDlg::validateT2DurationInput(CString sT2Duration)
{
	static const int VALID_INPUT_LENGTH = 0x02;

	if(!checkIfInputIsNotEmpty(sT2Duration))
	{
		m_EditBoxT2Duration.SetFocus();
		throw ValidationException("Please key-in hex value for T2 Duration Toggle Blinking State.");
	}
	
	if(!checkIfInputLengthIsValid(sT2Duration,VALID_INPUT_LENGTH))
	{
		m_EditBoxT2Duration.SetFocus();
		throw ValidationException("Invalid input length. Length must be 2.");
	}

	if(!checkIfInputIsInHexFormat(sT2Duration))
	{
		m_EditBoxT2Duration.SetFocus();
		throw ValidationException("Please key-in hex value for T2 Duration Toggle Blinking State.");
	}
}

void CACR122UBasicDeviceProgrammingDlg::validateNumberOfRepetitionInput(CString sNumberOfRepetition)
{
	static const int VALID_INPUT_LENGTH = 0x02;

	if(!checkIfInputIsNotEmpty(sNumberOfRepetition))
	{
		m_EditBoxNumberOfRepetitions.SetFocus();
		throw ValidationException("Please key-in hex value for Number of Repetition.");
	}
	
	if(!checkIfInputLengthIsValid(sNumberOfRepetition,VALID_INPUT_LENGTH))
	{
		m_EditBoxNumberOfRepetitions.SetFocus();
		throw ValidationException("Invalid input length. Length must be 2.");
	}

	if(!checkIfInputIsInHexFormat(sNumberOfRepetition))
	{
		m_EditBoxNumberOfRepetitions.SetFocus();
		throw ValidationException("Please key-in hex value for Number of Repetition.");
	}
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonRedFinalOn() 
{
	m_RadioButtonRedFinalOff.SetCheck(false);	
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonRedFinalOff() 
{
	m_RadioButtonRedFinalOn.SetCheck(false);	
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonRedStateMaskOn() 
{
	m_RadioButtonRedStateMaskOff.SetCheck(false);
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonRedStateMaskOff() 
{
	m_RadioButtonRedStateMaskOn.SetCheck(false);
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonRedInitBlinkingStateOn() 
{
	m_RadioButtonRedInitBlinkingStateOff.SetCheck(false);
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonRedInitBlinkingStateOff() 
{
	m_RadioButtonRedInitBlinkingStateOn.SetCheck(false);
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonRedBlinkingMaskOn() 
{
	m_RadioButtonRedBlinkingMaskOff.SetCheck(false);
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonRedBlinkingMaskOff() 
{
	m_RadioButtonRedBlinkingMaskOn.SetCheck(false);
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonGreenFinalStateOn() 
{
	m_RadioButtonGreenFinalStateOff.SetCheck(false);
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonGreenFinalStateOff() 
{
	m_RadioButtonGreenFinalStateOn.SetCheck(false);
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonGreenStateMaskOn() 
{
	m_RadioButtonGreenStateMaskOff.SetCheck(false);
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonGreenStateMaskOff() 
{
	m_RadioButtonGreenStateMaskOn.SetCheck(false);
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonGreenInitBlinkingStateOn() 
{
	m_RadioButtonGreenInitBlinkingStateOff.SetCheck(false);	
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonGreenInitBlinkingStateOff() 
{
	m_RadioButtonGreenInitBlinkingStateOn.SetCheck(false);
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonGreenBlinkingMaskOn() 
{
	m_RadioButtonGreenBlinkingMaskOff.SetCheck(false);
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonGreenBlinkingMaskOff() 
{
	m_RadioButtonGreenBlinkingMaskOn.SetCheck(false);	
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonLinkToBuzzerOff() 
{
	m_RadioButtonT2Duration.SetCheck(false);
	m_RadioButtonT1Duration.SetCheck(false);
	m_RadioButtonT1AndT2Duration.SetCheck(false);	
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonT2Duration() 
{
	m_RadioButtonLinkToBuzzerOff.SetCheck(false);
	m_RadioButtonT1Duration.SetCheck(false);
	m_RadioButtonT1AndT2Duration.SetCheck(false);
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonT1Duration() 
{
	m_RadioButtonLinkToBuzzerOff.SetCheck(false);
	m_RadioButtonT2Duration.SetCheck(false);
	m_RadioButtonT1AndT2Duration.SetCheck(false);	
}

void CACR122UBasicDeviceProgrammingDlg::onRadioButtonT1AndT2Duration() 
{
	m_RadioButtonLinkToBuzzerOff.SetCheck(false);
	m_RadioButtonT2Duration.SetCheck(false);
	m_RadioButtonT1Duration.SetCheck(false);	
}

void CACR122UBasicDeviceProgrammingDlg::onButtonClear() 
{
	m_EditBoxApduLogs.SetWindowText("");
}

void CACR122UBasicDeviceProgrammingDlg::onButtonReset() 
{
	if(pPcscReader_->isConnectionActive())
		pPcscReader_->disconnect();	

	pPcscReader_->cleanup();
	resetControls();	
}

void CACR122UBasicDeviceProgrammingDlg::onButtonQuit() 
{
	CDialog::OnOK();	
}
