#ifndef AIGINE_H
#define AIGINE_H

#pragma warning (disable: 4786)


#define DIRECTINPUT_VERSION (0x0800)
#include <Windows.h>
#include <StdIO.h>
#include <Sys\\Stat.h>
#include <Math.h>
#include <D3D9.h>
#include <D3DX9.h>
#include <DInput.h>
#include <DSound.h>
#include <DShow.h>
#include <DXErr9.h>
#include "../tools/DShowBaseClasses/Streams.h"
/**/

#include <GL/glaux.h>
#include <GL/glut.h>

#include <iostream>
#include <string>
#include <map>
//using namespace std;


#ifndef BYTE
	typedef unsigned char	BYTE;
#endif
#ifndef DWORD
	typedef unsigned long	DWORD;
#endif
#ifndef BOOL
	typedef int	BOOL;
#endif


// Allgemeine Makros und Definitionen
#define AGE_SAFE_DELETE(x)		{if((x) != NULL) {delete (x); (x) = NULL;}}		// Sicheres Löschen
#define AGE_SAFE_DELETE_ARRAY(x)	{if((x) != NULL) {delete[] (x); (x) = NULL;}}	// Sicheres Löschen eines Arrays
#define AGE_SAFE_RELEASE(x)		{if((x) != NULL) {(x)->Release(); (x) = NULL;}}	// Sicheres Releasen eines COM-Objekts
#define AGE_SAFE_MEMFREE(x)		{if((x) != NULL) {MemFree((x)); (x) = NULL;}}	// Sicheres Freigeben von Speicher
#define AGE_MIN(a, b)			((a) < (b) ? (a) : (b))							// Minimum
#define AGE_MAX(a, b)			((a) > (b) ? (a) : (b))							// Maximum
#define AGE_PI					(3.1415926535897932384626433832795f)			// Pi
#define AGE_DEG_TO_RAD(x)		((x) * 0.0174532925199432957692369076848f)		// Grad -> Bogenmaß
#define AGE_RAD_TO_DEG(x)		((x) * 57.295779513082320876798154814105f)		// Bogenmaß -> Grad
#define AGE_COLOR_CONV (0.003921568627450980392156862745098f) // = 1/255
	
#if _MSC_VER >= 1300
#define __AGE_FUNCTION__ (__FUNCTION__)
#else
#define __AGE_FUNCTION__ ("(Unbekannt)")
#endif
	
	// Makros zum Debuggen
#define AGE_INFO(x) {WriteToLog("<tr><td><font size=\"2\"><b><font color=\"#008000\">INFO:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (x), RemoveDir(__FILE__), __LINE__, __AGE_FUNCTION__);}
#define AGE_WARNING(x) {WriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">WARNUNG:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (x), RemoveDir(__FILE__), __LINE__, __AGE_FUNCTION__);}
#define AGE_ERROR_MESSAGE(x) {WriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (x), RemoveDir(__FILE__), __LINE__, __AGE_FUNCTION__);}
#define AGE_ERROR(x, r) {WriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (x), RemoveDir(__FILE__), __LINE__, __AGE_FUNCTION__); return (r);}
#define AGE_ERROR_NULL_POINTER(x, r) {WriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> <i>%s</i> ist NULL!</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (x), RemoveDir(__FILE__), __LINE__, __AGE_FUNCTION__); return (r);}
#define AGE_ERROR_INVALID_VALUE(x, r) {WriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> <i>%s</i> hat einen ungültigen Wert!</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (x), RemoveDir(__FILE__), __LINE__, __AGE_FUNCTION__); return (r);}
#define AGE_ERROR_OUT_OF_MEMORY(r) {WriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> Nicht genug Speicher!</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", RemoveDir(__FILE__), __LINE__, __AGE_FUNCTION__); return (r);}
#define AGE_ERROR_FILE(f, r) {WriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> Die Datei <i>%s</i> konnte nicht geöffnet, gelesen, erstellt oder beschrieben werden!</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (f), RemoveDir(__FILE__), __LINE__, __AGE_FUNCTION__); return (r);}
#define AGE_ERROR_RESOURCE(n, t, r) {WriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> Fehler beim Zugreifen auf die Ressource <i>%d</i> vom Typ <i>%d</i>!</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (n), (t), RemoveDir(__FILE__), __LINE__, __AGE_FUNCTION__); return (r);}
#define AGE_ERROR_DIRECTX(f, x, r) {WriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> Der Aufruf von <i>%s</i> verursachte den DirectX-Fehler <i>%s</i>! Beschreibung: <i>%s</i></font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (f), DXGetErrorString9((x)), DXGetErrorDescription9((x)), RemoveDir(__FILE__), __LINE__, __AGE_FUNCTION__); return (r);}

const enum Result {
	AGE_OK			= 0,	// Erfolg
	AGE_ERROR		= 1,	// Allgemeiner Fehler
	AGE_CANCELED	= 2,	// Aktion abgebrochen
	AGE_NOT_FOUND	= 3,	// Nicht gefunden
	AGE_STOP		= 4,	// Stopp (Vorgang abbrechen)
};

//float FloatRandom(const float fMin, const float fMax);



#include "Utils/Knot.h"
#include "Utils/List.h"
#include "Utils/Tree.h"
#include "Utils/fileControl.h"
#include "Utils/Memory.h"
#include "Utils/Utils.h"
#include "Utils/File.h"
#include "Utils/VFile.h"

#include "Control/Keyboard.h"
#include "Control/Mouse.h"


#include "../Tools/loader3DS/Object3DS.h"



#include "Model/Vector2D.h"
#include "Model/Vector3D.h"
#include "Model/Color.h"
#include "Model/Translation3D.h"
#include "Model/Rotation3D.h"
#include "Model/Scale3D.h"
#include "Model/Matrix4x4.h"

#include "Utils/Log.h"


#include "Model/IndexBuffer.h"


//#include "View/Effect.h"
//#include "Model/Model.h"
//#include "View/Config.h"
//#include "View/Direct3D.h"



#include "Model/Mesh.h"
#include "Model/Object3D.h"
#include "Model/Object.h"
#include "Model/AiGinEObject.h"
#include "Model/LookAtObjects.h"
#include "View/Camera.h"
#include "View/GraphicPipeline.h"
#include "Model/SceneGraph.h"
#include "Model/SceneManagement.h"
#include "Model/AgeObjects/ageObject3DS.h"
#include "Model/AgeObjects/ageObjectPrim.h"


#include "Model/MeshList.h"
#include "Model/ObjectList.h"
#include "Model/Plane.h"
#include "Model/Shader.h"



#include "View/Light.h"
#include "View/Renderer.h"
#include "View/Viewport.h"
	
/*
#include "../Tools/loader3DS/CLoad3DS.h"
#include "../Tools/loader3DS/Types3DS.h"
*/

#endif //AIGINE_H
