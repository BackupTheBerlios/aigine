# Microsoft Developer Studio Project File - Name="TriBase" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TriBase - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "TriBase.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "TriBase.mak" CFG="TriBase - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "TriBase - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TriBase - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TriBase - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TRIBASE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /Op /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TRIBASE_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 d3d9.lib d3dx9.lib dinput8.lib dsound.lib strmiids.lib strmbase.lib dxerr9.lib dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# Begin Special Build Tool
OutDir=.\Release
TargetDir=.\Release
TargetPath=.\Release\TriBase.dll
SOURCE="$(InputPath)"
PostBuild_Cmds=COPY $(TargetPath) Bin	COPY $(TargetDir)\TriBase.lib Lib	COPY $(OutDir)\TriBase.dll $(WinDir)\System32
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TriBase - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TriBase___Win32_Debug"
# PROP BASE Intermediate_Dir "TriBase___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TRIBASE_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TRIBASE_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 d3d9.lib d3dx9d.lib dinput8.lib dsound.lib strmiids.lib strmbased.lib dxerr9.lib dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"Debug/TriBaseD.dll" /pdbtype:sept
# Begin Special Build Tool
OutDir=.\Debug
TargetDir=.\Debug
TargetPath=.\Debug\TriBaseD.dll
SOURCE="$(InputPath)"
PostBuild_Cmds=COPY $(TargetPath) Bin	COPY $(TargetDir)\TriBaseD.lib Lib	COPY $(OutDir)\TriBaseD.dll $(WinDir)\System32
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "TriBase - Win32 Release"
# Name "TriBase - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Src\tb2DSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tb2DVolumeSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tb2DPrimitives.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tb3DSReader.cpp
# ADD CPP /Zp1
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Src\tbButtonHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbColor.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbDirect3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbDirectInput.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbDirectSound.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbDraw2D.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbDraw2D_16.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbDraw2D_16A.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbDraw2D_32.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbDynList.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbFile.cpp
# ADD CPP /Zp1
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Src\tbFont.cpp
# ADD CPP /Zp1
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Src\tbGUI.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbGUIButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbGUICheckBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbGUIFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbGUIImage.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbGUIInput.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbGUIList.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbGUIRadioBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbGUIText.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbIndexBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbINIReader.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbList.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbLog.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbModel.cpp
# ADD CPP /Zp1
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Src\tbMusic.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbOctree.cpp
# ADD CPP /Zp1
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Src\tbParticleSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbPlane.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbShadowVolume.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbSkyBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbSound.cpp
# ADD CPP /Zp1
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Src\tbSpriteEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbTextureManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbVector2.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbVector3.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbVector4.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbVertexBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbVFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\tbVideo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\TriBase.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Res\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Include\tb2DSprite.h
# End Source File
# Begin Source File

SOURCE=.\Include\tb2DVolumeSprite.h
# End Source File
# Begin Source File

SOURCE=.\Include\tb2DPrimitives.h
# End Source File
# Begin Source File

SOURCE=.\Include\tb3DSReader.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbButtonHandler.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbColor.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbConfig.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbDirect3D.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbDirectInput.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbDirectSound.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbDraw2D.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbDynList.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbEffect.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbFile.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbFont.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbGUI.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbIndexBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbINIReader.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbList.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbLog.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbMatrix.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbMemory.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbModel.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbMusic.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbObject.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbOctree.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbParticleSystem.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbPlane.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbShadowVolume.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbSkyBox.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbSound.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbSpriteEngine.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbTextureManager.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbUtils.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbVector2.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbVector3.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbVector4.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbVertexBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbVFile.h
# End Source File
# Begin Source File

SOURCE=.\Include\tbVideo.h
# End Source File
# Begin Source File

SOURCE=.\Include\TriBase.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\Bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Icon1.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Resource.rc
# End Source File
# End Group
# Begin Group "DirectX-Dateien"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\activecf.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\amaudio.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\amparse.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\amstream.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\amva.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\Amvideo.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\atsmedia.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\audevcod.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\austream.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\aviriff.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\basetsd.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\bdaiface.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\bdamedia.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\Bdatif.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\bdatypes.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\comlite.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\control.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3d.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3d8.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3d8caps.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3d8types.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3d9.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3d9caps.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3d9types.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dcaps.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3drm.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3drmdef.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3drmobj.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3drmwin.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dtypes.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dvec.inl"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dx.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dx8.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dx8core.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dx8effect.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dx8math.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dx8math.inl"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dx8mesh.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dx8shape.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dx8tex.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dx9.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dx9anim.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dx9core.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dx9effect.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dx9math.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dx9math.inl"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dx9mesh.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dx9shader.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dx9shape.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dx9tex.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dxcore.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dxerr.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dxmath.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dxmath.inl"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dxshapes.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\d3dxsprite.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\ddraw.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\ddstream.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dinput.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dinputd.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dls1.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dls2.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dmdls.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dmerror.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dmksctrl.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dmo.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dmodshow.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dmoimpl.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dmoreg.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dmort.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dmplugin.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dmusbuff.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dmusicc.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dmusicf.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dmusici.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dmusics.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dpaddr.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dplay.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dplay8.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dplobby.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dplobby8.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dpnathlp.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dsconf.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dsetup.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\DShow.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dshowasf.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dsound.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dv.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dvdevcod.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dvdmedia.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dvoice.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dvp.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dx7todx8.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\DxDiag.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dxerr8.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dxerr9.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dxfile.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dxtrans.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\dxva.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\edevdefs.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\errors.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\evcode.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\il21dec.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\Iwstdec.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\ks.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\ksguid.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\ksmedia.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\ksproxy.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\ksuuids.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\mediaerr.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\mediaobj.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\medparam.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\mixerocx.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\mmstream.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\mpconfig.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\Mpeg2Bits.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\mpeg2data.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\Mpeg2Error.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\mpegtype.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\Mstvca.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\Mstve.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\Msvidctl.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\multimon.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\playlist.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\qedit.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\qnetwork.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\regbag.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\rmxfguid.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\rmxftmpl.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\sbe.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\Segment.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\strmif.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\tune.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\tuner.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\tvratings.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\uuids.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\vfwmsgs.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\videoacc.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\vmr9.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\vpconfig.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\vpnotify.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\vptype.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\xprtdefs.h"
# End Source File
# End Group
# End Target
# End Project
