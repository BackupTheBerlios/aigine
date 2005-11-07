/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`�__\ /\ \ \ \  __\ /\ __\_ /\  __\   /�__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbButtonHandler.h
	=================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit Tasten und Kn�pfen

	Autor:
	Snorky
	[24.9.2003]

********************************************************************/

#ifndef __TBBUTTONHANDLER__
#define __TBBUTTONHANDLER__


// ******************************************************************
// Statusflags f�r die R�ckruffunktionen
#define TB_BH_NOTPRESSED	(0)
#define TB_BH_PRESSED		(1)
#define TB_BH_GETPRESSED	(2)
#define TB_BH_RELEASED		(4)

// ******************************************************************
// Klasse f�r das Keyboard-Verhalten
class TRIBASE_API tbButtonHandler
{
private:
	// Variablen
	BOOL* m_pbButtonsCurrent;				// aktueller Kn�pfe Status
	BOOL* m_pbButtonsPast;					// alter Kn�pfe Status
	DWORD m_dwNumButtons;					// Anzahl der Kn�pfe
	tbResult (**m_pCallback)(DWORD dwKey, DWORD dwFlags);	// Array von R�ckruffunktionen
	tbVector2 m_vMouseRelative;				// relative Mausbewegung
	tbVector2 m_vMouseIntern;				// interne Mausposition
	tbVector2 m_vMouse;						// Mausposition auf dem Monitor
	tbVector2 m_vMouseScaling;				// Mausskalierung
	
public:
	// Konstruktor und Destruktor
	tbButtonHandler();
	~tbButtonHandler();
	
	// Methoden
	tbResult Exit();						// alles freigeben und zur�cksetzen
	tbResult Init(DWORD dwButtons);			// festlegen wieviele Kn�pfe es geben soll
	tbResult GetState(BOOL* pbKeys, float* pfKeys = NULL);		// aktualisieren der Buttons

	BOOL AnyKeyPressed();					// ist irgendeine Taste gedr�ckt?
	BOOL AnyKeyGetPressed();				// wird irgendeine Taste gedr�ckt?
	BOOL AnyKeyReleased();					// wird irgendeine Taste losgelassen?

	BOOL AnyMouseButtonPressed();			// ist irgendeine Maus-Taste gedr�ckt?
	BOOL AnyMouseButtonGetPressed();		// wird irgendeine Maus-Taste gedr�ckt?
	BOOL AnyMouseButtonReleased();			// wird irgendeine Maus-Taste losgelassen?

	tbResult SetCallback(DWORD dwKey, tbResult (*pFunc)(DWORD, DWORD));	// Setzen einer R�ckruffunktion
	tbResult Process();						// Alle R�ckruffunktionen ausf�hren

	// Inline-Methoden
	// ist die Taste gedr�ckt?
	BOOL KeyPressed(DWORD dwKey)	{return (m_pbButtonsCurrent[dwKey]);}
	// wird die Taste gerade gedr�ckt?
	BOOL KeyGetPressed(DWORD dwKey) {return (!(m_pbButtonsPast[dwKey]) && (m_pbButtonsCurrent[dwKey]));}
	// wird die Taste gerade losgelassen?
	BOOL KeyReleased(DWORD dwKey)	{return ((m_pbButtonsPast[dwKey]) && !(m_pbButtonsCurrent[dwKey]));}

	// Mausbuttons
	BOOL MouseLeftPressed()		{return (m_pbButtonsCurrent[TB_MOUSE_BUTTON(0)]);}
	BOOL MouseLeftGetPressed()	{return (!(m_pbButtonsPast[TB_MOUSE_BUTTON(0)]) && (m_pbButtonsCurrent[TB_MOUSE_BUTTON(0)]));}
	BOOL MouseLeftReleased()	{return ((m_pbButtonsPast[TB_MOUSE_BUTTON(0)]) && !(m_pbButtonsCurrent[TB_MOUSE_BUTTON(0)]));}

	BOOL MouseRightPressed()	{return (m_pbButtonsCurrent[TB_MOUSE_BUTTON(1)]);}
	BOOL MouseRightGetPressed()	{return (!(m_pbButtonsPast[TB_MOUSE_BUTTON(1)]) && (m_pbButtonsCurrent[TB_MOUSE_BUTTON(1)]));}
	BOOL MouseRightReleased()	{return ((m_pbButtonsPast[TB_MOUSE_BUTTON(1)]) && !(m_pbButtonsCurrent[TB_MOUSE_BUTTON(1)]));}

	BOOL MouseMiddlePressed()	{return (m_pbButtonsCurrent[TB_MOUSE_BUTTON(2)]);}
	BOOL MouseMiddleGetPressed(){return (!(m_pbButtonsPast[TB_MOUSE_BUTTON(2)]) && (m_pbButtonsCurrent[TB_MOUSE_BUTTON(2)]));}
	BOOL MouseMiddleReleased()	{return ((m_pbButtonsPast[TB_MOUSE_BUTTON(2)]) && !(m_pbButtonsCurrent[TB_MOUSE_BUTTON(2)]));}


	BOOL* GetButtonsCurrent()	{return m_pbButtonsCurrent;}
	BOOL* GetButtonsPast()		{return m_pbButtonsPast;}
	DWORD GetNumButtons()		{return m_dwNumButtons;}
	tbVector2 GetMouse()		{return m_vMouse;}
	void SetMouse(tbVector2 vCoords) {m_vMouseIntern = vCoords;}
	tbVector2 GetMouseScaling()	{return m_vMouseScaling;}
	void SetMouseScaling(tbVector2 vScaling) {m_vMouseScaling = vScaling;}
	tbVector2 GetRelativeMouse() { return m_vMouseRelative; }

};

// ******************************************************************

#endif	__TBBUTTONHANDLER__
