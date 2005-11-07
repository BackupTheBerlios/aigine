# Microsoft Developer Studio Project File - Name="Galactica" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Galactica - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "Galactica.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "Galactica.mak" CFG="Galactica - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "Galactica - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "Galactica - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Galactica - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 tribase.lib d3d9.lib d3dx9.lib dxerr9.lib dxguid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Galactica - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 tribased.lib d3d9.lib d3dx9d.lib dxerr9.lib dxguid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Galactica - Win32 Release"
# Name "Galactica - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Galactica.cpp
# End Source File
# Begin Source File

SOURCE=.\Game.cpp
# End Source File
# Begin Source File

SOURCE=.\Intro.cpp
# End Source File
# Begin Source File

SOURCE=.\MainMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\Projectile.cpp
# End Source File
# Begin Source File

SOURCE=.\Ship.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Galactica.h
# End Source File
# Begin Source File

SOURCE=.\Game.h
# End Source File
# Begin Source File

SOURCE=.\Intro.h
# End Source File
# Begin Source File

SOURCE=.\MainMenu.h
# End Source File
# Begin Source File

SOURCE=.\Projectile.h
# End Source File
# Begin Source File

SOURCE=.\Ship.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Galactica.rc
# End Source File
# Begin Source File

SOURCE=.\Icon1.ico
# End Source File
# End Group
# Begin Group "TriBase-Dateien"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\TriBase\Include\tb3DSReader.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbColor.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbConfig.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbDirect3D.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbDirectInput.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbDirectSound.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbDraw2D.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbEffect.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbFile.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbFont.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbIndexBuffer.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbList.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbLog.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbMatrix.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbMemory.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbModel.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbMusic.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbObject.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbParticleSystem.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbPlane.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbSkyBox.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbSound.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbSpriteEngine.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbTextureManager.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbUtils.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbVector2.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbVector3.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbVertexBuffer.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\tbVFile.h
# End Source File
# Begin Source File

SOURCE=..\TriBase\Include\TriBase.h
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

SOURCE="..\..\..\Programmieren\DirectX 9 SDK\Include\edevctrl.h"
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
