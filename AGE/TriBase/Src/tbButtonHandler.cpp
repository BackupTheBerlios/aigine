/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbButtonHandler.cpp
	===================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit Tasten und Knöpfen

	Autor:
	Snorky
	[24.9.2003]

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Konsturktor
tbButtonHandler::tbButtonHandler()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbButtonHandler));
}

// ******************************************************************
// Destruktor
tbButtonHandler::~tbButtonHandler()
{
	Exit();
}

// ******************************************************************
// alles freigeben und zurücksetzen
tbResult tbButtonHandler::Exit()
{
	// Arrays freigeben
	TB_SAFE_DELETE_ARRAY(m_pbButtonsCurrent);
	TB_SAFE_DELETE_ARRAY(m_pbButtonsPast);
	TB_SAFE_DELETE_ARRAY(m_pCallback);

	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbButtonHandler));

	return TB_OK;
}

// ******************************************************************
// festlegen wieviele Knöpfe es geben soll
tbResult tbButtonHandler::Init(DWORD dwButtons)
{
	// sicherheitshalber alles vorher freigeben
	Exit();

	// Knopfanzahl setzen
	m_dwNumButtons = dwButtons;

	// Speicher für aktuelle Knöpfe belegen
	m_pbButtonsCurrent = new BOOL [m_dwNumButtons];
	if(m_pbButtonsCurrent == NULL)
	{
		// Fehler!
		TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	}

	// Speicher für vergangene Knöpfe belegen
	m_pbButtonsPast = new BOOL [m_dwNumButtons];
	if(m_pbButtonsPast == NULL)
	{
		// Fehler!
		TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	}

	// Speicher für Rückruffunktionen der Knöpfe belegen
	m_pCallback = new (tbResult (*[m_dwNumButtons])(DWORD, DWORD));
	if(m_pCallback == NULL)
	{
		// Fehler!
		TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	}

	// Tasten mit FALSE vorbelegen
	ZeroMemory((void*)m_pbButtonsCurrent, m_dwNumButtons * sizeof(BOOL));
	ZeroMemory((void*)m_pbButtonsPast, m_dwNumButtons * sizeof(BOOL));
	ZeroMemory((void*)m_pCallback, m_dwNumButtons * sizeof(tbResult (*)(DWORD, DWORD)));

	return TB_OK;
}

// ******************************************************************
// aktualisieren der Buttons
tbResult tbButtonHandler::GetState(BOOL* pbKeys, float* pfKeys)
{
	// aktuellen Stand zum alten machen
	memcpy(m_pbButtonsPast, m_pbButtonsCurrent, m_dwNumButtons*sizeof(BOOL));

	// aktuellen Stand durch neuen ersetzen
	memcpy(m_pbButtonsCurrent, pbKeys, m_dwNumButtons*sizeof(BOOL));

	// Mauskoordinaten updaten
	if(pfKeys != NULL)
	{
		// relative Verschiebung ermitteln
		m_vMouseRelative = tbVector2(pfKeys[TB_MOUSE_X_POS] - pfKeys[TB_MOUSE_X_NEG], pfKeys[TB_MOUSE_Y_POS] - pfKeys[TB_MOUSE_Y_NEG]);
		m_vMouseRelative *= m_vMouseScaling;

		// interne Mausposition mit Kommastellen berechnen
		m_vMouseIntern += m_vMouseRelative;

		// Prüfen ob Maus noch im Bildschirm ist
		if(m_vMouseIntern.x < 0) m_vMouseIntern.x = 0;
		if(m_vMouseIntern.x > tbDirect3D::GetScreenSize().x -1.0f) m_vMouseIntern.x = tbDirect3D::GetScreenSize().x -1.0f;
		if(m_vMouseIntern.y < 0) m_vMouseIntern.y = 0;
		if(m_vMouseIntern.y > tbDirect3D::GetScreenSize().y -1.0f) m_vMouseIntern.y = tbDirect3D::GetScreenSize().y -1.0f;

		// interne Mausposition in ganze Zahlen umwandeln
		m_vMouse.x = float((long)m_vMouseIntern.x);
		m_vMouse.y = float((long)m_vMouseIntern.y);
	}

	return TB_OK;
}

// ******************************************************************
// ist irgendeine Taste gedrückt?
BOOL tbButtonHandler::AnyKeyPressed()
{
	DWORD dw;

	// Alle Tastaturtasten durchgehen
	for(dw = 0; dw <= 107; dw++)
	{
		if(m_pbButtonsCurrent[dw])
			return TRUE;
	}

	// Maustasten auch checken
	for(dw = 114; dw <= 121; dw++)
	{
		if(m_pbButtonsCurrent[dw])
			return TRUE;
	}

	// nichts gefunden
	return FALSE;
}

// ******************************************************************
// wird irgendeine Taste gedrückt?
BOOL tbButtonHandler::AnyKeyGetPressed()
{
	DWORD dw;

	// Alle Tastaturtasten durchgehen
	for(dw = 0; dw <= 107; dw++)
	{
		if(!(m_pbButtonsPast[dw]) && m_pbButtonsCurrent[dw])
			return TRUE;
	}

	// Maustasten auch checken
	for(dw = 114; dw <= 121; dw++)
	{
		if(!(m_pbButtonsPast[dw]) && m_pbButtonsCurrent[dw])
			return TRUE;
	}

	// nichts gefunden
	return FALSE;
}

// ******************************************************************
// wird irgendeine Taste losgelassen?
BOOL tbButtonHandler::AnyKeyReleased()
{
	DWORD dw;

	// Alle Tastaturtasten durchgehen
	for(dw = 0; dw <= 107; dw++)
	{
		if(m_pbButtonsPast[dw] && !(m_pbButtonsCurrent[dw]))
			return TRUE;
	}

	// Maustasten auch checken
	for(dw = 114; dw <= 121; dw++)
	{
		if(m_pbButtonsPast[dw] && !(m_pbButtonsCurrent[dw]))
			return TRUE;
	}

	// nichts gefunden
	return FALSE;
}

// ******************************************************************
// ist irgendeine Maus-Taste gedrückt?
BOOL tbButtonHandler::AnyMouseButtonPressed()
{
	DWORD dw;

	// Alle Buttons durchgehen
	for(dw = 0; dw <= 2; dw++)
	{
		if(m_pbButtonsCurrent[TB_MOUSE_BUTTON(dw)])
			return TRUE;
	}

	// nichts gefunden
	return FALSE;
}

// ******************************************************************
// wird irgendeine Maus-Taste gedrückt?
BOOL tbButtonHandler::AnyMouseButtonGetPressed()
{
	DWORD dw;

	// Alle Buttons durchgehen
	for(dw = 0; dw <= 2; dw++)
	{
		if(!(m_pbButtonsPast[TB_MOUSE_BUTTON(dw)]) && m_pbButtonsCurrent[TB_MOUSE_BUTTON(dw)])
			return TRUE;
	}

	// nichts gefunden
	return FALSE;
}

// ******************************************************************
// wird irgendeine Maus-Taste losgelassen?
BOOL tbButtonHandler::AnyMouseButtonReleased()
{
	DWORD dw;

	// Alle Buttons durchgehen
	for(dw = 0; dw <= 2; dw++)
	{
		if(m_pbButtonsPast[TB_MOUSE_BUTTON(dw)] && !(m_pbButtonsCurrent[TB_MOUSE_BUTTON(dw)]))
			return TRUE;
	}

	// nichts gefunden
	return FALSE;
}

// ******************************************************************
// Setzen einer Rückruffunktion
tbResult tbButtonHandler::SetCallback(DWORD dwKey, tbResult (*pFunc)(DWORD, DWORD))
{
	if(dwKey < 0 || dwKey >= m_dwNumButtons) return TB_ERROR;

	m_pCallback[dwKey] = pFunc;

	return TB_OK;
}

// ******************************************************************
// Alle Rückruffunktionen ausführen
tbResult tbButtonHandler::Process()
{
	tbResult r;
	DWORD dwFlags;

	// Alle Knöpfe durchgehen
	for(DWORD dw = 0; dw < m_dwNumButtons; dw++)
	{
		// Hat der Knopf eine Rückruffunktion?
		if(m_pCallback[dw] != NULL)
		{
			// Flags setzen
			dwFlags = TB_BH_NOTPRESSED;
			if(KeyPressed(dw)) dwFlags |= TB_BH_PRESSED;
			if(KeyGetPressed(dw)) dwFlags |= TB_BH_GETPRESSED;
			if(KeyReleased(dw)) dwFlags |= TB_BH_RELEASED;
			
			// Rückruffunktion ausführen
			r = m_pCallback[dw](dw, dwFlags);

			// Auf Fehler prüfen
			if(r != TB_OK) TB_ERROR("Fehler bei einer ButtonHandler-Rückruffunktion!",TB_ERROR);
		}
	}

	return TB_OK;
}

// ******************************************************************

