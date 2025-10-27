; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=NtagNDEFSampleCodeDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "NtagNDEFSampleCode.h"
LastPage=0

ClassCount=11
Class1=CNtagNDEFSampleCodeApp
Class2=CNtagNDEFSampleCodeDoc
Class3=CNtagNDEFSampleCodeView
Class4=CMainFrame

ResourceCount=7
Resource1=IDD_DialogVCard
Resource2=IDR_MAINFRAME
Resource3=IDR_NTAGNDTYPE
Resource5=IDD_DialogUrl
Class5=CChildFrame
Class6=CAboutDlg
Resource4=IDD_DialogText
Class7=NdefSmartPosterDlg
Class8=NdefTextDlg
Class9=NdefUriDlg
Class10=NdefVCardDlg
Resource6=IDD_DialogSmartPoster
Class11=NtagNDEFSampleCodeDlg
Resource7=IDD_DialogNtagNDEFSampleCode

[CLS:CNtagNDEFSampleCodeApp]
Type=0
HeaderFile=NtagNDEFSampleCode.h
ImplementationFile=NtagNDEFSampleCode.cpp
Filter=N

[CLS:CNtagNDEFSampleCodeDoc]
Type=0
HeaderFile=NtagNDEFSampleCodeDoc.h
ImplementationFile=NtagNDEFSampleCodeDoc.cpp
Filter=N

[CLS:CNtagNDEFSampleCodeView]
Type=0
HeaderFile=NtagNDEFSampleCodeView.h
ImplementationFile=NtagNDEFSampleCodeView.cpp
Filter=C


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M


[CLS:CAboutDlg]
Type=0
HeaderFile=NtagNDEFSampleCode.cpp
ImplementationFile=NtagNDEFSampleCode.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_DialogSmartPoster]
Type=1
Class=NdefSmartPosterDlg
ControlCount=10
Control1=IDC_ButtonEncodeSmartPoster,button,1476460545
Control2=IDC_StaticSmartPosterTitle,static,1342308352
Control3=IDC_ComboBoxSmartPosterTitle,combobox,1478557699
Control4=IDC_StaticSmartPosterUri,static,1342308352
Control5=IDC_ComboBoxUri,combobox,1478557699
Control6=IDC_EditSmartPosterTitle,edit,1484849280
Control7=IDC_EditSmartPosterUri,edit,1484849280
Control8=IDC_StaticSmartPosterAction,static,1342308352
Control9=IDC_ComboBoxAction,combobox,1478557699
Control10=IDC_StaticSmartPosterRequiredFields,static,1342308352

[DLG:IDD_DialogText]
Type=1
Class=NdefTextDlg
ControlCount=8
Control1=IDC_ButtonEncodeText,button,1476460545
Control2=IDC_ComboBoxLanguage,combobox,1478557699
Control3=IDC_StaticTextLanguage,static,1342308352
Control4=IDC_StaticTextMessage,static,1342308352
Control5=IDC_EditMessage,edit,1486946308
Control6=IDC_RadioUtf8,button,1476395017
Control7=IDC_RadioUtf16,button,1476395017
Control8=IDC_StaticTextRequireFields,static,1342308352

[DLG:IDD_DialogUrl]
Type=1
Class=NdefUriDlg
ControlCount=5
Control1=IDC_ButtonEncodeUrl,button,1476460545
Control2=IDC_StaticUri,static,1342308352
Control3=IDC_ComboBoxUriUri,combobox,1478557699
Control4=IDC_EditUriUri,edit,1484849280
Control5=IDC_StaticRequiredFields,static,1342308352

[DLG:IDD_DialogVCard]
Type=1
Class=NdefVCardDlg
ControlCount=8
Control1=IDC_ButtonEncodeVCard,button,1476460545
Control2=IDC_StaticGivenName,static,1342308352
Control3=IDC_EditGivenName,edit,1484849280
Control4=IDC_StaticMiddleName,static,1342308352
Control5=IDC_StaticSurname,static,1342308352
Control6=IDC_EditMiddleName,edit,1484849280
Control7=IDC_EditSurname,edit,1484849280
Control8=IDC_StaticVCardRequiredFields,static,1342308352

[CLS:NdefSmartPosterDlg]
Type=0
HeaderFile=NdefSmartPosterDlg.h
ImplementationFile=NdefSmartPosterDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=NdefSmartPosterDlg
VirtualFilter=dWC

[CLS:NdefTextDlg]
Type=0
HeaderFile=NdefTextDlg.h
ImplementationFile=NdefTextDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_RadioUtf8

[CLS:NdefUriDlg]
Type=0
HeaderFile=NdefUriDlg.h
ImplementationFile=NdefUriDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_ButtonEncodeUrl

[CLS:NdefVCardDlg]
Type=0
HeaderFile=NdefVCardDlg.h
ImplementationFile=NdefVCardDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_ButtonEncodeVCard

[DLG:IDD_DialogNtagNDEFSampleCode]
Type=1
Class=NtagNDEFSampleCodeDlg
ControlCount=14
Control1=IDC_STATIC,static,1342308352
Control2=IDC_ComboBoxReaderList,combobox,1344340227
Control3=IDC_ButtonInitialize,button,1342242816
Control4=IDC_ButtonConnect,button,1476460544
Control5=IDC_TabMain,SysTabControl32,1342177280
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EditNdefMessage,edit,1352730628
Control8=IDC_ButtonWriteToCard,button,1476460544
Control9=IDC_ButtonReadFromCard,button,1476460544
Control10=IDC_STATIC,static,1342308352
Control11=IDC_RichEditApduLogs,RICHEDIT,1353779396
Control12=IDC_ButtonClear,button,1342242816
Control13=IDC_ButtonReset,button,1342242816
Control14=IDC_ButtonQuit,button,1342242816

[CLS:NtagNDEFSampleCodeDlg]
Type=0
HeaderFile=NtagNDEFSampleCodeDlg.h
ImplementationFile=NtagNDEFSampleCodeDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EditNdefMessage

