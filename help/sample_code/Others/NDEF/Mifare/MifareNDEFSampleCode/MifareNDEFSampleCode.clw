; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CNdefVCard
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MifareNDEFSampleCode.h"

ClassCount=7
Class1=CMifareNDEFSampleCodeApp
Class2=CMifareNDEFSampleCodeDlg
Class3=CAboutDlg

ResourceCount=7
Resource1=IDD_DialogSmartPoster
Resource2=IDR_MAINFRAME
Resource3=IDD_DialogMifareNdefSampleCode
Class4=CNdefSmartPoster
Resource4=IDD_AboutBox
Class5=CNdefText
Resource5=IDD_DialogUri
Class6=CNdefUri
Resource6=IDD_DialogText
Class7=CNdefVCard
Resource7=IDD_DialogVCard

[CLS:CMifareNDEFSampleCodeApp]
Type=0
HeaderFile=MifareNDEFSampleCode.h
ImplementationFile=MifareNDEFSampleCode.cpp
Filter=N

[CLS:CMifareNDEFSampleCodeDlg]
Type=0
HeaderFile=MifareNDEFSampleCodeDlg.h
ImplementationFile=MifareNDEFSampleCodeDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_BUTTON_INITIALIZE

[CLS:CAboutDlg]
Type=0
HeaderFile=MifareNDEFSampleCodeDlg.h
ImplementationFile=MifareNDEFSampleCodeDlg.cpp
Filter=D
LastObject=CAboutDlg

[CLS:CNdefSmartPoster]
Type=0
HeaderFile=NdefSmartPoster.h
ImplementationFile=NdefSmartPoster.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDITSMARTPOSTERURI

[CLS:CNdefText]
Type=0
HeaderFile=NdefText.h
ImplementationFile=NdefText.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_BUTTONENCODETEXT

[CLS:CNdefUri]
Type=0
HeaderFile=NdefUri.h
ImplementationFile=NdefUri.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDITURLURI

[CLS:CNdefVCard]
Type=0
HeaderFile=NdefVCard.h
ImplementationFile=NdefVCard.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CNdefVCard

[DLG:IDD_DialogSmartPoster]
Type=1
Class=CNdefSmartPoster
ControlCount=10
Control1=IDC_STATICSMARTPOSTERTITLE,static,1342308352
Control2=IDC_COMBOSMARTPOSTERTITLELANGUAGE,combobox,1344339971
Control3=IDC_EDITSMARTPOSTERTITLE,edit,1350631552
Control4=IDC_STATICSMARTPOSTERURI,static,1342308352
Control5=IDC_COMBOSMARTPOSTERPREFIXURI,combobox,1344339971
Control6=IDC_EDITSMARTPOSTERURI,edit,1350631552
Control7=IDC_STATICSMARTPOSTERACTION,static,1342308352
Control8=IDC_COMBOSMARTPOSTERACTION,combobox,1344340227
Control9=IDC_STATICSMARTPOSTERREQUIRED,static,1342308352
Control10=IDC_BUTTONENCODESMARTPOSTER,button,1342242817

[DLG:IDD_DialogText]
Type=1
Class=CNdefText
ControlCount=8
Control1=IDC_STATICTEXTLANGUAGE,static,1342308352
Control2=IDC_COMBOTEXTLANGUAGE,combobox,1344339971
Control3=IDC_RADIOTEXTUTF8,button,1342177289
Control4=IDC_RADIOTEXTUTF16,button,1342177289
Control5=IDC_STATICTEXTMESSAGE,static,1342308352
Control6=IDC_EDITTEXTMESSAGE,edit,1352728580
Control7=IDC_STATIC_REQUIRED,static,1342308352
Control8=IDC_BUTTONENCODETEXT,button,1342242817

[DLG:IDD_DialogUri]
Type=1
Class=CNdefUri
ControlCount=5
Control1=IDC_STATICURLURI,static,1342308352
Control2=IDC_COMBOURLURI,combobox,1344274435
Control3=IDC_EDITURLURI,edit,1350631552
Control4=IDC_STATIC_REQUIRED,static,1342308352
Control5=IDC_BUTTONENCODEURL,button,1342242817

[DLG:IDD_DialogVCard]
Type=1
Class=CNdefVCard
ControlCount=8
Control1=IDC_STATICVCARDGIVEN,static,1342308352
Control2=IDC_EDITVCARDGIVEN,edit,1350631552
Control3=IDC_STATICVCARDMIDDLE,static,1342308352
Control4=IDC_EDITVCARDMIDDLE,edit,1350631552
Control5=IDC_STATICVCARDSUR,static,1342308352
Control6=IDC_EDITVCARDSUR,edit,1350631552
Control7=IDC_STATIC_REQUIRED,static,1342308352
Control8=IDC_BUTTONENCODEVCARD,button,1342242817

[DLG:IDD_DialogMifareNdefSampleCode]
Type=1
Class=CMifareNDEFSampleCodeDlg
ControlCount=30
Control1=IDC_STATIC_SELECTREADER,static,1342308352
Control2=IDC_COMBO_READERS,combobox,1344340227
Control3=IDC_STATIC_APDULOGS,static,1342308352
Control4=IDC_BUTTON_INITIALIZE,button,1342242816
Control5=IDC_BUTTON_CONNECT,button,1342242816
Control6=IDC_EDIT_APDULOGS,edit,1352730692
Control7=IDC_MY_TAB,SysTabControl32,1342177280
Control8=IDC_STATIC_NDEFMESSAGE,static,1342308352
Control9=IDC_EDIT_NDEFMESSAGE,edit,1350633476
Control10=IDC_STATIC_GROUPBOX,button,1342177287
Control11=IDC_STATIC_MAD,static,1342308352
Control12=IDC_EDIT_MADKEYA,edit,1350633601
Control13=IDC_EDIT_MADACCESSBITS,edit,1350633601
Control14=IDC_EDIT_MADKEYB,edit,1350631553
Control15=IDC_STATIC_MADKEYA,static,1342308352
Control16=IDC_STATIC_MADACCESSBITS,static,1342308352
Control17=IDC_STATIC_MADKEYB,static,1342308352
Control18=IDC_STATIC_NFC,static,1342308352
Control19=IDC_EDIT_NFCKEYA,edit,1350633601
Control20=IDC_EDIT_NFCACCESSBITS,edit,1350633601
Control21=IDC_EDIT_NFCKEYB,edit,1350631553
Control22=IDC_STATIC_NFCKEYA,static,1342308352
Control23=IDC_STATIC_NFCACCESSBITS,static,1342308352
Control24=IDC_STATIC_NFCKEYB,static,1342308352
Control25=IDC_BUTTON_WRITETOCARD,button,1342242816
Control26=IDC_BUTTON_READFROMCARD,button,1342242816
Control27=IDC_BUTTON_RESETCARD,button,1342242816
Control28=IDC_BUTTON_CLEARLOGS,button,1342242816
Control29=IDC_BUTTON_RESET,button,1342242816
Control30=IDC_BUTTON_QUIT,button,1342242816

[DLG:IDD_AboutBox]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

