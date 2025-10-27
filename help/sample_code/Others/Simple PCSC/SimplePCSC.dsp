# Microsoft Developer Studio Project File - Name="SimplePCSC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SimplePCSC - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SimplePCSC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SimplePCSC.mak" CFG="SimplePCSC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SimplePCSC - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SimplePCSC - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SimplePCSC - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "./AcsCommon" /I "./AcsCommon/Apdu" /I "./AcsCommon/Osa" /I "./AcsCommon/Helper" /I "./AcsReader" /I "./AcsReader/PcscReader" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
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

!ELSEIF  "$(CFG)" == "SimplePCSC - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./AcsCommon" /I "./AcsCommon/Apdu" /I "./AcsCommon/Osa" /I "./AcsCommon/Helper" /I "./AcsReader" /I "./AcsReader/PcscReader" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/Simple PCSC.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SimplePCSC - Win32 Release"
# Name "SimplePCSC - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "AcsCommon"

# PROP Default_Filter ""
# Begin Group "Apdu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AcsCommon\Apdu\Apdu.cpp
# End Source File
# End Group
# Begin Group "Osa"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AcsCommon\Osa\osa.cpp
# End Source File
# End Group
# Begin Group "Helper"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AcsCommon\Helper\Helper.cpp
# End Source File
# Begin Source File

SOURCE=.\AcsCommon\Helper\HelperClass.cpp
# End Source File
# End Group
# End Group
# Begin Group "AcsReader"

# PROP Default_Filter ""
# Begin Group "PcscReader"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AcsReader\PcscReader\PcscException.cpp
# End Source File
# Begin Source File

SOURCE=.\AcsReader\PcscReader\PcscPolling.cpp
# End Source File
# Begin Source File

SOURCE=.\AcsReader\PcscReader\PcscReader.cpp
# End Source File
# Begin Source File

SOURCE=.\AcsReader\PcscReader\PcscReaderName.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\AcsReader\AcsReader.cpp
# End Source File
# Begin Source File

SOURCE=".\AcsReader\AcsReaderException .cpp"
# End Source File
# End Group
# Begin Source File

SOURCE=.\SimplePCSC.cpp
# End Source File
# Begin Source File

SOURCE=.\SimplePCSC.rc
# End Source File
# Begin Source File

SOURCE=.\SimplePCSCDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AcsCommon\AcsCommon.h
# End Source File
# Begin Source File

SOURCE=.\AcsReader\AcsReader.h
# End Source File
# Begin Source File

SOURCE=.\AcsReader\AcsReaderException.h
# End Source File
# Begin Source File

SOURCE=.\AcsCommon\Apdu\Apdu.h
# End Source File
# Begin Source File

SOURCE=.\AcsCommon\Functor.h
# End Source File
# Begin Source File

SOURCE=.\AcsCommon\Helper\Helper.h
# End Source File
# Begin Source File

SOURCE=.\AcsCommon\Helper\HelperClass.h
# End Source File
# Begin Source File

SOURCE=.\AcsCommon\Osa\osa.h
# End Source File
# Begin Source File

SOURCE=.\AcsReader\PcscReader\PcscException.h
# End Source File
# Begin Source File

SOURCE=.\AcsReader\PcscReader\PcscReader.h
# End Source File
# Begin Source File

SOURCE=.\AcsReader\PcscReader\PcscReaderName.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SimplePCSC.h
# End Source File
# Begin Source File

SOURCE=.\SimplePCSCDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\AcsReader\PcscReader\WINSCARD.H
# End Source File
# Begin Source File

SOURCE=.\AcsReader\PcscReader\Winscard.lib
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ACS_Small.ico
# End Source File
# Begin Source File

SOURCE=.\res\SimplePCSC.ico
# End Source File
# Begin Source File

SOURCE=.\res\SimplePCSC.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
