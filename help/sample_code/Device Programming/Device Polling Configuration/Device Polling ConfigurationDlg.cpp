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
#include "Device Polling Configuration.h"
#include "Device Polling ConfigurationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDevicePollingConfigurationDlg *_pThis;
Helper _cHelper;

void onSendCommand(const byte *pSendData, const int iLength)
{
	_pThis->addMessageToLog((CString)"\r\n<< " + (CString)(_cHelper.byteAsString(pSendData, iLength,true)));
}

void onReceiveCommand(const byte *pReceiveData, const int iLength)
{
	_pThis->addMessageToLog((CString)">> " + (CString)(_cHelper.byteAsString(pReceiveData, iLength,true)));
}

CDevicePollingConfigurationDlg::CDevicePollingConfigurationDlg(CWnd* pParent)
	: CDialog(CDevicePollingConfigurationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDevicePollingConfigurationDlg)
	m_EditBoxApduLogsText = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_ACSLOGO);
}

CDevicePollingConfigurationDlg::~CDevicePollingConfigurationDlg()
{
	if(pPcscReader_->isConnectionActive())
		pPcscReader_->disconnect();	

	pPcscReader_->cleanup();
	delete pPcscReader_;
	delete pAcr122u_;
}

void CDevicePollingConfigurationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDevicePollingConfigurationDlg)
	DDX_Control(pDX, IDC_RadioButton500ms, m_RadioButton500ms);
	DDX_Control(pDX, IDC_RadioButton250ms, m_RadioButton250ms);
	DDX_Control(pDX, IDC_GroupBoxPollingInterval, m_GroupBoxPollingInterval);
	DDX_Control(pDX, IDC_CheckBoxDetectTopaz, m_CheckBoxDetectTopaz);
	DDX_Control(pDX, IDC_CheckBoxDetectIso14443TypeB, m_CheckBoxDetectIso14443TypeB);
	DDX_Control(pDX, IDC_CheckBoxDetectIso14443TypeA, m_CheckBoxDetectIso14443TypeA);
	DDX_Control(pDX, IDC_CheckBoxDetectFelica424, m_CheckBoxDetectFelica424);
	DDX_Control(pDX, IDC_CheckBoxDetectFelica212, m_CheckBoxDetectFelica212);
	DDX_Control(pDX, IDC_CheckBoxAutoPiccPolling, m_CheckBoxAutoPiccPolling);
	DDX_Control(pDX, IDC_CheckBoxAutoAtsGeneration, m_CheckBoxAutoAtsGeneration);
	DDX_Control(pDX, IDC_GroupBoxOperatingParameters, m_GroupBoxOperatingParameters);
	DDX_Control(pDX, IDC_EditBoxApduLogs, m_EditBoxApduLogs);
	DDX_Control(pDX, IDC_ComboBoxReaderList, m_ComboBoxReaderList);
	DDX_Control(pDX, IDC_ButtonSetParameters, m_ButtonSetParameters);
	DDX_Control(pDX, IDC_ButtonReset, m_ButtonReset);
	DDX_Control(pDX, IDC_ButtonConnect, m_ButtonConnect);
	DDX_Control(pDX, IDC_ButtonClear, m_ButtonClear);
	DDX_Control(pDX, IDC_ButtonGetParameters, m_ButtonGetParameters);
	DDX_Control(pDX, IDC_ButtonQuit, m_ButtonQuit);
	DDX_Control(pDX, IDC_ButtonInitialize, m_ButtonInitialize);
	DDX_Text(pDX, IDC_EditBoxApduLogs, m_EditBoxApduLogsText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDevicePollingConfigurationDlg, CDialog)
	//{{AFX_MSG_MAP(CDevicePollingConfigurationDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ButtonClear, onButtonClear)
	ON_BN_CLICKED(IDC_ButtonInitialize, onButtonInitialize)
	ON_BN_CLICKED(IDC_ButtonQuit, onButtonQuit)
	ON_BN_CLICKED(IDC_ButtonConnect, onButtonConnect)
	ON_BN_CLICKED(IDC_RadioButton250ms, onRadioButton250ms)
	ON_BN_CLICKED(IDC_RadioButton500ms, onRadioButton500ms)
	ON_BN_CLICKED(IDC_ButtonReset, onButtonReset)
	ON_BN_CLICKED(IDC_ButtonGetParameters, onButtonGetParameters)
	ON_BN_CLICKED(IDC_ButtonSetParameters, onButtonSetParameters)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDevicePollingConfigurationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	
	_pThis = this;

	resetControl();

	pPcscReader_ = new PcscReader();
	pAcr122u_ = new Acr122u(pPcscReader_);

	return TRUE;
}

void CDevicePollingConfigurationDlg::OnPaint() 
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

HCURSOR CDevicePollingConfigurationDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDevicePollingConfigurationDlg::resetControl()
{
	enableControls(false);

    m_CheckBoxAutoAtsGeneration.SetCheck(false);
    m_CheckBoxAutoPiccPolling.SetCheck(false);
    m_CheckBoxDetectFelica212.SetCheck(false);
    m_CheckBoxDetectFelica424.SetCheck(false);
    m_CheckBoxDetectIso14443TypeA.SetCheck(false);
    m_CheckBoxDetectIso14443TypeB.SetCheck(false);
    m_CheckBoxDetectTopaz.SetCheck(false);
    m_RadioButton250ms.SetCheck(true);
    m_RadioButton500ms.SetCheck(false);

    m_ButtonConnect.EnableWindow(false);
    m_ComboBoxReaderList.ResetContent();

    m_EditBoxApduLogs.SetWindowText("");

    addMessageToLog("Program Ready");
}

void CDevicePollingConfigurationDlg::enableControls(bool bIsEnabled)
{
	m_GroupBoxOperatingParameters.EnableWindow(bIsEnabled);
	m_CheckBoxAutoAtsGeneration.EnableWindow(bIsEnabled);
    m_CheckBoxAutoPiccPolling.EnableWindow(bIsEnabled);
    m_CheckBoxDetectFelica212.EnableWindow(bIsEnabled);
    m_CheckBoxDetectFelica424.EnableWindow(bIsEnabled);
    m_CheckBoxDetectIso14443TypeA.EnableWindow(bIsEnabled);
    m_CheckBoxDetectIso14443TypeB.EnableWindow(bIsEnabled);
    m_CheckBoxDetectTopaz.EnableWindow(bIsEnabled);

	m_GroupBoxPollingInterval.EnableWindow(bIsEnabled);
	m_RadioButton250ms.EnableWindow(bIsEnabled);
	m_RadioButton500ms.EnableWindow(bIsEnabled);
	m_ButtonGetParameters.EnableWindow(bIsEnabled);
	m_ButtonSetParameters.EnableWindow(bIsEnabled);
}

void CDevicePollingConfigurationDlg::addMessageToLog(CString sMessage)
{
	UpdateData(true);
	sMessage = sMessage + "\r\n";
	m_EditBoxApduLogsText += sMessage;
	UpdateData(false);

	m_EditBoxApduLogs.SetSel(0xffff, 0xffff);
}

void CDevicePollingConfigurationDlg::onPcscException(PcscException cException)
{
	string sLogMessage;

	sprintf(aMessage_,"[%ld] %s",cException.getErrorCode(),cException.getMessage());
	sLogMessage = "\r\n" + string(aMessage_);
	addMessageToLog(sLogMessage.c_str());
	MessageBox(aMessage_, "Error", MB_ICONERROR);
}

void CDevicePollingConfigurationDlg::onStandardException(const std::exception& cException)
{
	MessageBox(cException.what(), "Error", MB_ICONERROR);
	addMessageToLog(cException.what());
}

void CDevicePollingConfigurationDlg::onGeneralException()
{
	MessageBox("General exception occured.", "Error", MB_ICONERROR);
	addMessageToLog("General exception occured.");
}

void CDevicePollingConfigurationDlg::onRadioButton250ms() 
{
	m_RadioButton500ms.SetCheck(false);	
}

void CDevicePollingConfigurationDlg::onRadioButton500ms() 
{
	m_RadioButton250ms.SetCheck(false);	
}

void CDevicePollingConfigurationDlg::onButtonInitialize() 
{
	const int READER_COUNT = 0x0A;
	string *pReaderList = new string[READER_COUNT];
	uint8 uReaderCount = READER_COUNT;

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
		displayReaderListToComboBox(pReaderList,uReaderCount);
		addMessageToLog("\r\nInitialize success");
		m_ComboBoxReaderList.SetCurSel(0);
		m_ButtonConnect.EnableWindow(true);
		enableControls(false);
	}
	catch (PcscException cException) { onInitializeException(); onPcscException(cException); }
	catch(const std::exception &cException) { onInitializeException(); onStandardException(cException); }
	catch(...) { onInitializeException(); onGeneralException(); }

	delete[] pReaderList;
}

void CDevicePollingConfigurationDlg::onInitializeException()
{
	m_ComboBoxReaderList.ResetContent();
	enableControls(false);
}

void CDevicePollingConfigurationDlg::displayReaderListToComboBox(string* pList, int32 iListSize)
{
	int32 iIterator;

	for(iIterator = 0; iIterator < iListSize; ++iIterator)
		m_ComboBoxReaderList.AddString(pList[iIterator].data());
}

void CDevicePollingConfigurationDlg::onButtonConnect() 
{
	CString sReaderName;

	try
	{
		m_ComboBoxReaderList.GetWindowText(sReaderName);
		validateSelectedReaderName(sReaderName);
		pPcscReader_->connectDirect((CString)sReaderName);
		sprintf(aMessage_, "\r\n%s %s", "Successfully connected to ", sReaderName);
		addMessageToLog(aMessage_);
		enableControls(true);
        m_ButtonConnect.EnableWindow(false);
	}
	catch (PcscException cException) { onPcscException(cException); }
	catch(const std::exception &cException) { onStandardException(cException); }
	catch(...) { onGeneralException(); }
}

void CDevicePollingConfigurationDlg::validateSelectedReaderName(CString sReaderName)
{
	if (!strcmp(sReaderName, ""))
		throw ValidationException("Please select smartcard reader");
}

void CDevicePollingConfigurationDlg::onButtonGetParameters() 
{
	PiccOperatingParameter *pPiccOperatingParameter;
            
    try
    {
        addMessageToLog("\r\nGet PICC Operating Parameter");
        pPiccOperatingParameter = pAcr122u_->getPiccOperatingParameter();
		uncheckAllCheckBox();
		select250msInterval();
		updateControlState(pPiccOperatingParameter);

		delete pPiccOperatingParameter;
    }
	catch (PcscException cException) { onPcscException(cException); }
	catch(const std::exception &cException) { onStandardException(cException); }
	catch(...) { onGeneralException(); }
}

void CDevicePollingConfigurationDlg::updateControlState(PiccOperatingParameter *pPiccOperatingParameter)
{
	if (pPiccOperatingParameter->getAutoAtsGeneration() == ATS_GENERATION_ENABLED)
        m_CheckBoxAutoAtsGeneration.SetCheck(true);                
    if (pPiccOperatingParameter->getAutoPiccPolling() == PICC_POLLING_ENABLED)
        m_CheckBoxAutoPiccPolling.SetCheck(true);
    if (pPiccOperatingParameter->getPollingInterval() == POLLING_INTERVAL_MS_500)
		select500msInterval();
    if (pPiccOperatingParameter->getFelica424() == PARAMETER_OPTION_DETECT)
        m_CheckBoxDetectFelica424.SetCheck(true);
    if (pPiccOperatingParameter->getFelica212() == PARAMETER_OPTION_DETECT)
        m_CheckBoxDetectFelica212.SetCheck(true);
    if (pPiccOperatingParameter->getTopaz() == PARAMETER_OPTION_DETECT)
        m_CheckBoxDetectTopaz.SetCheck(true);
    if (pPiccOperatingParameter->getIso14443TypeB() == PARAMETER_OPTION_DETECT)
        m_CheckBoxDetectIso14443TypeB.SetCheck(true);
    if (pPiccOperatingParameter->getIso14443TypeA() == PARAMETER_OPTION_DETECT)
        m_CheckBoxDetectIso14443TypeA.SetCheck(true);
}

void CDevicePollingConfigurationDlg::uncheckAllCheckBox()
{
	m_CheckBoxAutoAtsGeneration.SetCheck(false);
    m_CheckBoxAutoPiccPolling.SetCheck(false);
    m_CheckBoxDetectFelica212.SetCheck(false);
    m_CheckBoxDetectFelica424.SetCheck(false);
    m_CheckBoxDetectIso14443TypeA.SetCheck(false);
    m_CheckBoxDetectIso14443TypeB.SetCheck(false);
    m_CheckBoxDetectTopaz.SetCheck(false);
}

void CDevicePollingConfigurationDlg::select250msInterval()
{
	m_RadioButton250ms.SetCheck(true);
	m_RadioButton500ms.SetCheck(false);
}

void CDevicePollingConfigurationDlg::select500msInterval()
{
	m_RadioButton500ms.SetCheck(true);
	m_RadioButton250ms.SetCheck(false);
}

void CDevicePollingConfigurationDlg::onButtonSetParameters() 
{
	PiccOperatingParameter *pPiccOperatingParameter;

    try
    {
        pPiccOperatingParameter = new PiccOperatingParameter();
        addMessageToLog("\r\nSet PICC Operating Parameter");
		configureParameters(pPiccOperatingParameter);
		delete pAcr122u_->setPiccOperatingParameter(pPiccOperatingParameter);
    }
	catch (PcscException cException) { onPcscException(cException); }
	catch(const std::exception &cException) { onStandardException(cException); }
	catch(...) { onGeneralException(); }

	delete pPiccOperatingParameter;
}

void CDevicePollingConfigurationDlg::configureParameters(PiccOperatingParameter *pPiccOperatingParameter)
{
	const int CHECKED = 0x01;

	if(m_CheckBoxAutoPiccPolling.GetCheck() == CHECKED)
            pPiccOperatingParameter->setAutoPiccPolling(PICC_POLLING_ENABLED);
    if(m_CheckBoxAutoAtsGeneration.GetCheck() == CHECKED)
        pPiccOperatingParameter->setAutoAtsGeneration(ATS_GENERATION_ENABLED);
    if(m_CheckBoxDetectFelica212.GetCheck() == CHECKED)
        pPiccOperatingParameter->setFelica212(PARAMETER_OPTION_DETECT);
    if(m_CheckBoxDetectFelica424.GetCheck() == CHECKED)
        pPiccOperatingParameter->setFelica424(PARAMETER_OPTION_DETECT);
    if(m_CheckBoxDetectIso14443TypeA.GetCheck() == CHECKED)
        pPiccOperatingParameter->setIso14443TypeA(PARAMETER_OPTION_DETECT);
    if(m_CheckBoxDetectIso14443TypeB.GetCheck() == CHECKED)
        pPiccOperatingParameter->setIso14443TypeB(PARAMETER_OPTION_DETECT);
    if(m_CheckBoxDetectTopaz.GetCheck() == CHECKED)
        pPiccOperatingParameter->setTopaz(PARAMETER_OPTION_DETECT);
	if(m_RadioButton250ms.GetCheck() == CHECKED)
		pPiccOperatingParameter->setPollingInterval(POLLING_INTERVAL_MS_250);
	else
		pPiccOperatingParameter->setPollingInterval(POLLING_INTERVAL_MS_500);
}

void CDevicePollingConfigurationDlg::onButtonClear() 
{
	m_EditBoxApduLogs.SetWindowText("");	
}

void CDevicePollingConfigurationDlg::onButtonReset() 
{
	if(pPcscReader_->isConnectionActive())
		pPcscReader_->disconnect();	

	pPcscReader_->cleanup();
	resetControl();
}

void CDevicePollingConfigurationDlg::onButtonQuit() 
{
	CDialog::OnOK();	
}
