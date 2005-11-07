/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	TriBase.h
	=========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Deklaration allgemeiner Funktionen

	Autor:
	David Scherfgen

	Bearbeitet:
	Snorky
	[24.9.03]

********************************************************************/

#ifdef TRIBASE_EXPORTS
#define TRIBASE_API __declspec(dllexport)
#else
#define TRIBASE_API __declspec(dllimport)
#endif

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
#include <DShowBaseClasses\\Streams.h>

#ifdef _DEBUG
	#define TB_DLL_HANDLE (GetModuleHandle("TriBaseD.dll"))
#else
	#define TB_DLL_HANDLE (GetModuleHandle("TriBase.dll"))
#endif

// Allgemeine Makros und Definitionen
#define TB_SAFE_DELETE(x)		{if((x) != NULL) {delete (x); (x) = NULL;}}		// Sicheres Löschen
#define TB_SAFE_DELETE_ARRAY(x)	{if((x) != NULL) {delete[] (x); (x) = NULL;}}	// Sicheres Löschen eines Arrays
#define TB_SAFE_RELEASE(x)		{if((x) != NULL) {(x)->Release(); (x) = NULL;}}	// Sicheres Releasen eines COM-Objekts
#define TB_SAFE_MEMFREE(x)		{if((x) != NULL) {tbMemFree((x)); (x) = NULL;}}	// Sicheres Freigeben von Speicher
#define TB_MIN(a, b)			((a) < (b) ? (a) : (b))							// Minimum
#define TB_MAX(a, b)			((a) > (b) ? (a) : (b))							// Maximum
#define TB_PI					(3.1415926535897932384626433832795f)			// Pi
#define TB_DEG_TO_RAD(x)		((x) * 0.0174532925199432957692369076848f)		// Grad -> Bogenmaß
#define TB_RAD_TO_DEG(x)		((x) * 57.295779513082320876798154814105f)		// Bogenmaß -> Grad

#if _MSC_VER >= 1300
	#define __TB_FUNCTION__ (__FUNCTION__)
#else
	#define __TB_FUNCTION__ ("(Unbekannt)")
#endif

// Makros zum Debuggen
#define TB_INFO(x)						{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#008000\">INFO:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (x), tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__);}
#define TB_WARNING(x)					{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FFA000\">WARNUNG:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (x), tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__);}
#define TB_ERROR_MESSAGE(x)				{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (x), tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__);}
#define TB_ERROR(x, r)					{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (x), tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__); return (r);}
#define TB_ERROR_NULL_POINTER(x, r)		{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> <i>%s</i> ist NULL!</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (x), tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__); return (r);}
#define TB_ERROR_INVALID_VALUE(x, r)	{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> <i>%s</i> hat einen ungültigen Wert!</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (x), tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__); return (r);}
#define TB_ERROR_OUT_OF_MEMORY(r)		{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> Nicht genug Speicher!</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__); return (r);}
#define TB_ERROR_FILE(f, r)				{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> Die Datei <i>%s</i> konnte nicht geöffnet, gelesen, erstellt oder beschrieben werden!</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (f), tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__); return (r);}
#define TB_ERROR_RESOURCE(n, t, r)		{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> Fehler beim Zugreifen auf die Ressource <i>%d</i> vom Typ <i>%d</i>!</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (n), (t), tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__); return (r);}
#define TB_ERROR_DIRECTX(f, x, r)		{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> Der Aufruf von <i>%s</i> verursachte den DirectX-Fehler <i>%s</i>! Beschreibung: <i>%s</i></font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (f), DXGetErrorString9((x)), DXGetErrorDescription9((x)), tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__); return (r);}

// ******************************************************************
// Statuscodes
enum tbResult
{
	TB_OK			= 0,	// Erfolg
	TB_ERROR		= 1,	// Allgemeiner Fehler
	TB_CANCELED		= 2,	// Aktion abgebrochen
	TB_NOT_FOUND	= 3,	// Nicht gefunden
	TB_STOP			= 4,	// Stopp (Vorgang abbrechen)
};

// ******************************************************************
// Globale Variablen
extern HMODULE	tb_g_DLLHandle;			// Handle der TriBase-DLL-Datei
extern BOOL		tb_g_bInitCalled;		// Wurde tbInit aufgerufen?
extern BOOL		tb_g_bExitCalled;		// Wurde tbExit aufgerufen?
extern BOOL		tb_g_bAppActive;		// Ist das Anwendungsfenster aktiv?
extern LONGLONG	tb_g_llFrequency;		// Frequenz des Zeitgebers
extern double	tb_g_dFrequency;		// Frequenz des Zeitgebers (double)
extern float	tb_g_fTime;				// Zeitzähler
extern double	tb_g_dNumSecsPassed;	// Vergangene Sekunden seit letztem Frame
extern double	tb_g_dFramebrake;		// Framebremse, die maximale Framezahl umgerechnet mit Kehrwert

// ******************************************************************
// Funktionen deklarieren
TRIBASE_API tbResult tbInit();																			// Engine initialisieren
TRIBASE_API tbResult tbExit();																			// Engine herunterfahren
TRIBASE_API tbResult tbDoMessageLoop(tbResult (* pMoveProc)(float), 
									 tbResult (* pRenderProc)(float),
									 double dMaxFPS = 1000);											// Nachrichtenschleife
TRIBASE_API tbResult tbDelay(DWORD dwDuration);															// Pause machen
TRIBASE_API tbResult tbSetAppActive(BOOL bActive);														// Aktivierungsstatus der Anwendung setzen
TRIBASE_API	tbResult tbSetFramebrake(double dFramebrake = 0.0);												// Setzen der neuen Frameratenbegrenzung

// ******************************************************************

// Headerdateien der Engine einbinden
#include "..\\Res\\Resource.h"
#include "tbUtils.h"
#include "tbList.h"
#include "tbMemory.h"
#include "tbDynList.h"
#include "tbVector2.h"
#include "tbVector3.h"
#include "tbMatrix.h"
#include "tbPlane.h"
#include "tbColor.h"
#include "tbVector4.h"
#include "tbLog.h"
#include "tbFile.h"
#include "tbVFile.h"
#include "tb3DSReader.h"
#include "tbConfig.h"
#include "tbDirect3D.h"
#include "tbTextureManager.h"
#include "tbVertexBuffer.h"
#include "tbIndexBuffer.h"
#include "tbEffect.h"
#include "tbModel.h"
#include "tbFont.h"
#include "tbDirectInput.h"
#include "tbDirectSound.h"
#include "tbSound.h"
#include "tbMusic.h"
#include "tbDraw2D.h"
#include "tbSpriteEngine.h"
#include "tbSkyBox.h"
#include "tbObject.h"
#include "tbParticleSystem.h"
#include "tbGUI.h"
#include "tbShadowVolume.h"
#include "tbVideo.h"
#include "tbOctree.h"

#include "tbINIReader.h"
#include "tb2DPrimitives.h"
#include "tb2DSprite.h"
#include "tb2DVolumeSprite.h"
#include "tbButtonHandler.h"

// ******************************************************************