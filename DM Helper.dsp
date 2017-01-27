# Microsoft Developer Studio Project File - Name="DM Helper" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DM Helper - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DM Helper.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DM Helper.mak" CFG="DM Helper - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DM Helper - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DM Helper - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DM Helper - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "c:\gdi" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "c:\gdi" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 c:/DevStudio/VC98/lib/winmm.lib c:/gdi/Gdiplus.lib c:/gdi/Gdiplus.lib /nologo /subsystem:windows /machine:I386 /out:"Release/Dungeon Maestro.exe"

!ELSEIF  "$(CFG)" == "DM Helper - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "c:\gdi" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 c:/DevStudio/VC98/lib/winmm.lib c:/gdi/Gdiplus.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "DM Helper - Win32 Release"
# Name "DM Helper - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\cDMMapViewDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ColoredListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=".\DM Helper Common.cpp"
# End Source File
# Begin Source File

SOURCE=".\DM Helper.cpp"
# End Source File
# Begin Source File

SOURCE=".\DM Helper.rc"
# End Source File
# Begin Source File

SOURCE=".\DM HelperDlg.cpp"
# End Source File
# Begin Source File

SOURCE=.\DMCastSpellDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DMCharacterSelectorDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DMCharSheetDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DMCharSpellsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DMCharViewDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DMHPEditDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DMInventoryDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DMLoadFileDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DMMapEditDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DMModifyValueDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DMNPCViewDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DMPartyDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DMSFXEditorDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DMSFXMapperDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DMSoundboardDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DMSoundFXDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DMSpellDescDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DMTimeEditDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\cDMMapViewDialog.h
# End Source File
# Begin Source File

SOURCE=.\ColoredListCtrl.h
# End Source File
# Begin Source File

SOURCE=".\DM Helper Common.h"
# End Source File
# Begin Source File

SOURCE=".\DM Helper.h"
# End Source File
# Begin Source File

SOURCE=".\DM HelperDlg.h"
# End Source File
# Begin Source File

SOURCE=.\DMCastSpellDialog.h
# End Source File
# Begin Source File

SOURCE=.\DMCharacterSelectorDialog.h
# End Source File
# Begin Source File

SOURCE=.\DMCharSheetDialog.h
# End Source File
# Begin Source File

SOURCE=.\DMCharSpellsDialog.h
# End Source File
# Begin Source File

SOURCE=.\DMCharViewDialog.h
# End Source File
# Begin Source File

SOURCE=.\DMHPEditDialog.h
# End Source File
# Begin Source File

SOURCE=.\DMInventoryDialog.h
# End Source File
# Begin Source File

SOURCE=.\DMLoadFileDialog.h
# End Source File
# Begin Source File

SOURCE=.\DMMapEditDialog.h
# End Source File
# Begin Source File

SOURCE=.\DMModifyValueDialog.h
# End Source File
# Begin Source File

SOURCE=.\DMNPCViewDialog.h
# End Source File
# Begin Source File

SOURCE=.\DMPartyDialog.h
# End Source File
# Begin Source File

SOURCE=.\DMSFXEditorDialog.h
# End Source File
# Begin Source File

SOURCE=.\DMSFXMapperDialog.h
# End Source File
# Begin Source File

SOURCE=.\DMSoundboardDialog.h
# End Source File
# Begin Source File

SOURCE=.\DMSoundFXDialog.h
# End Source File
# Begin Source File

SOURCE=.\DMSpellDescDialog.h
# End Source File
# Begin Source File

SOURCE=.\DMTimeEditDialog.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\camera_icon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cleric.bmp
# End Source File
# Begin Source File

SOURCE=.\res\d20.ico
# End Source File
# Begin Source File

SOURCE=".\res\DM Helper.ico"
# End Source File
# Begin Source File

SOURCE=".\res\DM Helper.rc2"
# End Source File
# Begin Source File

SOURCE=.\res\DMGuide.bmp
# End Source File
# Begin Source File

SOURCE=.\res\down_arrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\down_arrow_pressed.bmp
# End Source File
# Begin Source File

SOURCE=.\res\expand_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\expand_up.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fighter.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Furyondy.bmp
# End Source File
# Begin Source File

SOURCE=.\res\inventory.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Keoland.bmp
# End Source File
# Begin Source File

SOURCE=.\res\left_icon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\magicuser.bmp
# End Source File
# Begin Source File

SOURCE=.\res\multiclass.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PlayersHandbook.bmp
# End Source File
# Begin Source File

SOURCE=.\res\print_icon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\right_icon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\save_icon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\spellbook.bmp
# End Source File
# Begin Source File

SOURCE=.\res\swap_icon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\thief.bmp
# End Source File
# Begin Source File

SOURCE=.\res\up_arrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\up_arrow_pressed.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\sounds\arrow_thump.wav
# End Source File
# Begin Source File

SOURCE=.\res\sounds\chant.wav
# End Source File
# Begin Source File

SOURCE=.\res\sounds\choir.wav
# End Source File
# Begin Source File

SOURCE=.\Release\Data\fonts\CloisterBlack.ttf
# End Source File
# Begin Source File

SOURCE=.\res\sounds\fireball.wav
# End Source File
# Begin Source File

SOURCE=.\res\sounds\ghost.wav
# End Source File
# Begin Source File

SOURCE=.\res\sounds\glass_break.wav
# End Source File
# Begin Source File

SOURCE=.\res\sounds\lightning.wav
# End Source File
# Begin Source File

SOURCE=.\res\sounds\magic_wand.wav
# End Source File
# Begin Source File

SOURCE=.\res\sounds\male_scream.wav
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\sounds\scream_female.wav
# End Source File
# Begin Source File

SOURCE=.\res\sounds\smashing.wav
# End Source File
# Begin Source File

SOURCE=.\res\sounds\squeak_door.wav
# End Source File
# Begin Source File

SOURCE=".\res\sounds\rpg_sound_pack\battle\sword-unsheathe.wav"
# End Source File
# Begin Source File

SOURCE=.\res\sounds\sword.wav
# End Source File
# Begin Source File

SOURCE=.\res\sounds\sword_fight.wav
# End Source File
# End Target
# End Project
