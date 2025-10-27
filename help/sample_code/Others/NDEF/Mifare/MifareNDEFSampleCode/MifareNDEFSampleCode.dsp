# Microsoft Developer Studio Project File - Name="MifareNDEFSampleCode" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MifareNDEFSampleCode - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MifareNDEFSampleCode.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MifareNDEFSampleCode.mak" CFG="MifareNDEFSampleCode - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MifareNDEFSampleCode - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MifareNDEFSampleCode - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MifareNDEFSampleCode - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "MifareNDEFSampleCode - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/Mifare NDEF Sample Code.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MifareNDEFSampleCode - Win32 Release"
# Name "MifareNDEFSampleCode - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AcsReader.cpp
# End Source File
# Begin Source File

SOURCE=.\Apdu.cpp
# End Source File
# Begin Source File

SOURCE=.\CardException.cpp
# End Source File
# Begin Source File

SOURCE=.\CardSelector.cpp
# End Source File
# Begin Source File

SOURCE=.\Helper.cpp
# End Source File
# Begin Source File

SOURCE=.\MifareClassic.cpp
# End Source File
# Begin Source File

SOURCE=.\MifareNDEFSampleCode.cpp
# End Source File
# Begin Source File

SOURCE=.\MifareNDEFSampleCode.rc
# End Source File
# Begin Source File

SOURCE=.\MifareNDEFSampleCodeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Ndef.cpp
# End Source File
# Begin Source File

SOURCE=.\NdefException.cpp
# End Source File
# Begin Source File

SOURCE=.\NdefMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\NdefRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\NdefRecordType.cpp
# End Source File
# Begin Source File

SOURCE=.\NdefSmartPoster.cpp
# End Source File
# Begin Source File

SOURCE=.\NdefText.cpp
# End Source File
# Begin Source File

SOURCE=.\NdefUri.cpp
# End Source File
# Begin Source File

SOURCE=.\NdefVCard.cpp
# End Source File
# Begin Source File

SOURCE=.\PcscException.cpp
# End Source File
# Begin Source File

SOURCE=.\PcscProvider.cpp
# End Source File
# Begin Source File

SOURCE=.\PcscReader.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\VCardIdentificationType.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ACSCommon.h
# End Source File
# Begin Source File

SOURCE=.\AcsReader.h
# End Source File
# Begin Source File

SOURCE=.\Apdu.h
# End Source File
# Begin Source File

SOURCE=.\CardException.h
# End Source File
# Begin Source File

SOURCE=.\CardSelector.h
# End Source File
# Begin Source File

SOURCE=.\Functor.h
# End Source File
# Begin Source File

SOURCE=.\Helper.h
# End Source File
# Begin Source File

SOURCE=.\MifareClassic.h
# End Source File
# Begin Source File

SOURCE=.\MifareNDEFSampleCode.h
# End Source File
# Begin Source File

SOURCE=.\MifareNDEFSampleCodeDlg.h
# End Source File
# Begin Source File

SOURCE=.\MyTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Ndef.h
# End Source File
# Begin Source File

SOURCE=.\NdefException.h
# End Source File
# Begin Source File

SOURCE=.\NdefMessage.h
# End Source File
# Begin Source File

SOURCE=.\NdefRecord.h
# End Source File
# Begin Source File

SOURCE=.\NdefRecordType.h
# End Source File
# Begin Source File

SOURCE=.\NdefSmartPoster.h
# End Source File
# Begin Source File

SOURCE=.\NdefText.h
# End Source File
# Begin Source File

SOURCE=.\NdefUri.h
# End Source File
# Begin Source File

SOURCE=.\NdefVCard.h
# End Source File
# Begin Source File

SOURCE=.\PcscException.h
# End Source File
# Begin Source File

SOURCE=.\PcscProvider.h
# End Source File
# Begin Source File

SOURCE=.\PcscReader.h
# End Source File
# Begin Source File

SOURCE=.\ReferenceData.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\VCardIdentificationType.h
# End Source File
# Begin Source File

SOURCE=.\WINSCARD.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\res\ACOS Sample Codes.ico"
# End Source File
# Begin Source File

SOURCE=.\res\ACS_Small.ico
# End Source File
# Begin Source File

SOURCE=.\res\MifareNDEFSampleCode.ico
# End Source File
# Begin Source File

SOURCE=.\res\MifareNDEFSampleCode.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\WINSCARD.LIB
# End Source File
# End Target
# End Project
