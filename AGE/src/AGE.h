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
//#include <DShowBaseClasses\\Streams.h>


// Allgemeine Makros und Definitionen
#define AGE_SAFE_DELETE(x)		{if((x) != NULL) {delete (x); (x) = NULL;}}		// Sicheres Löschen
#define AGE_SAFE_DELETE_ARRAY(x)	{if((x) != NULL) {delete[] (x); (x) = NULL;}}	// Sicheres Löschen eines Arrays
#define AGE_SAFE_RELEASE(x)		{if((x) != NULL) {(x)->Release(); (x) = NULL;}}	// Sicheres Releasen eines COM-Objekts
#define AGE_SAFE_MEMFREE(x)		{if((x) != NULL) {tbMemFree((x)); (x) = NULL;}}	// Sicheres Freigeben von Speicher
#define AGE_MIN(a, b)			((a) < (b) ? (a) : (b))							// Minimum
#define AGE_MAX(a, b)			((a) > (b) ? (a) : (b))							// Maximum
#define AGE_PI					(3.1415926535897932384626433832795f)			// Pi
#define AGE_DEG_TO_RAD(x)		((x) * 0.0174532925199432957692369076848f)		// Grad -> Bogenmaß
#define AGE_RAD_TO_DEG(x)		((x) * 57.295779513082320876798154814105f)		// Bogenmaß -> Grad

// Makros zum Debuggen
#define AGE_INFO(x)						{}//{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#008000\">INFO:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (x), tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__);}
#define AGE_WARNING(x)					{}//{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">WARNUNG:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (x), tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__);}
#define AGE_ERROR_MESSAGE(x)				{}//{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (x), tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__);}
#define AGE_ERROR(x, r)					{}//{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> %s</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (x), tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__); return (r);}
#define AGE_ERROR_NULL_POINTER(x, r)		{}//{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> <i>%s</i> ist NULL!</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (x), tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__); return (r);}
#define AGE_ERROR_INVALID_VALUE(x, r)	{}//{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> <i>%s</i> hat einen ungültigen Wert!</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (x), tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__); return (r);}
#define AGE_ERROR_OUT_OF_MEMORY(r)		{}//{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> Nicht genug Speicher!</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__); return (r);}
#define AGE_ERROR_FILE(f, r)				{}//{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> Die Datei <i>%s</i> konnte nicht geöffnet, gelesen, erstellt oder beschrieben werden!</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (f), tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__); return (r);}
#define AGE_ERROR_RESOURCE(n, t, r)		{}//{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> Fehler beim Zugreifen auf die Ressource <i>%d</i> vom Typ <i>%d</i>!</font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (n), (t), tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__); return (r);}
#define AGE_ERROR_DIRECTX(f, x, r)		{}//{tbWriteToLog("<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> Der Aufruf von <i>%s</i> verursachte den DirectX-Fehler <i>%s</i>! Beschreibung: <i>%s</i></font></td><td><font size=\"2\"> (<i>%s</i>, Zeile <i>%d</i>, Funktion <i>%s</i>)</font></td></tr>", (f), DXGetErrorString9((x)), DXGetErrorDescription9((x)), tbRemoveDir(__FILE__), __LINE__, __TB_FUNCTION__); return (r);}


// Statuscodes
enum age_Result
{
	AGE_OK			= 0,	// Erfolg
	AGE_ERROR		= 1,	// Allgemeiner Fehler
	AGE_CANCELED		= 2,	// Aktion abgebrochen
	AGE_NOT_FOUND	= 3,	// Nicht gefunden
	AGE_STOP			= 4,	// Stopp (Vorgang abbrechen)
};

// ******************************************************************
// Globale Variablen
//extern HMODULE	tb_g_DLLHandle;			// Handle der TriBase-DLL-Datei
extern BOOL		age_g_bInitCalled;		// Wurde tbInit aufgerufen?
extern BOOL		age_g_bExitCalled;		// Wurde tbExit aufgerufen?
extern BOOL		age_g_bAppActive;		// Ist das Anwendungsfenster aktiv?
extern LONGLONG	age_g_llFrequency;		// Frequenz des Zeitgebers
extern double	age_g_dFrequency;		// Frequenz des Zeitgebers (double)
extern float	age_g_fTime;				// Zeitzähler
extern double	age_g_dNumSecsPassed;	// Vergangene Sekunden seit letztem Frame

// ******************************************************************
// Funktionen deklarieren
TRIBASE_API age_Result age_Init();																			// Engine initialisieren
TRIBASE_API age_Result age_Exit();																			// Engine herunterfahren
TRIBASE_API age_Result age_DoMessageLoop(age_Result (* pMoveProc)(float), age_Result (* pRenderProc)(float));	// Nachrichtenschleife
TRIBASE_API age_Result age_Delay(DWORD dwDuration);															// Pause machen
TRIBASE_API age_Result age_SetAppActive(BOOL bActive);														// Aktivierungsstatus der Anwendung setzen

// ******************************************************************

// Headerdateien der Engine einbinden
#include "..\\Res\\Resource.h"
//#include "tbUtils.h"
#include "ageData\ageD_List.h"
#include "ageData\ageD_Memory.h"
//#include "tbDynList.h"
//#include "tbVector2.h"
//#include "tbVector3.h"
//#include "tbMatrix.h"
//#include "tbPlane.h"
//#include "tbColor.h"
//#include "tbLog.h"
//#include "tbFile.h"
#include "ageData\ageD_VFile.h"
//#include "tb3DSReader.h"
//#include "tbConfig.h"
//#include "tbDirect3D.h"
#include "ageData\ageD_D3DTextureManager.h"
#include "ageData\ageD_D3DVertexBuffer.h"
#include "ageData\ageD_D3DIndexBuffer.h"
#include "ageData\ageD_D3DEffect.h"
//#include "tbModel.h"
//#include "tbFont.h"
//#include "tbDirectInput.h"
//#include "tbDirectSound.h"
//#include "tbSound.h"
//#include "tbMusic.h"
//#include "tbDraw2D.h"
//#include "tbSpriteEngine.h"
//#include "tbSkyBox.h"
//#include "tbObject.h"
//#include "tbParticleSystem.h"
//#include "tbGUI.h"
//#include "tbShadowVolume.h"
//#include "tbVideo.h"
//#include "tbOctree.h"

// ******************************************************************