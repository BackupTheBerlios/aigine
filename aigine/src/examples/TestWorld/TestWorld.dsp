# Microsoft Developer Studio Project File - Name="TestWorld" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=TestWorld - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TestWorld.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TestWorld.mak" CFG="TestWorld - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TestWorld - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "TestWorld - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TestWorld - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "TestWorld - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib glaux.lib glut32.lib opengl32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "TestWorld - Win32 Release"
# Name "TestWorld - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\ageObject3DS.cpp
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\ageObjectPrim.cpp
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\AiGinEObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\View\Camera.cpp
# End Source File
# Begin Source File

SOURCE=..\..\tools\loader3DS\CLoad3DS.cpp
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\View\GraphicPipeline.cpp
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Control\Keyboard.cpp
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Utils\Knot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\Mesh.cpp
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Control\Mouse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\Object3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\tools\loader3DS\Object3DS.cpp
# End Source File
# Begin Source File

SOURCE=.\robot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\Rotation3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\Scale3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\SceneGraph.cpp
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\SceneManagement.cpp
# End Source File
# Begin Source File

SOURCE=TestWorld.cpp
# End Source File
# Begin Source File

SOURCE=..\..\tools\tgaLoader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\Translation3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Utils\Tree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\Vector2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\Vector3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\View\Viewport.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\ageObject3DS.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\ageObjectPrim.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\AiGinEObject.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\View\Camera.h
# End Source File
# Begin Source File

SOURCE=..\..\tools\loader3DS\CLoad3DS.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\View\GraphicPipeline.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Control\Keyboard.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Utils\Knot.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Utils\List.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\Mesh.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\MeshList.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Control\Mouse.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\Object3D.h
# End Source File
# Begin Source File

SOURCE=..\..\tools\loader3DS\Object3DS.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\ObjectList.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\View\Renderer.h
# End Source File
# Begin Source File

SOURCE=.\robot.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\Rotation3D.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\Scale3D.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\SceneGraph.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\SceneManagement.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\Shader.h
# End Source File
# Begin Source File

SOURCE=..\..\tools\tgaLoader.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\Translation3D.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Utils\Tree.h
# End Source File
# Begin Source File

SOURCE=..\..\tools\loader3DS\Types3DS.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\Vector2D.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\Model\Vector3D.h
# End Source File
# Begin Source File

SOURCE=..\..\AiGinEFramework\View\Viewport.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
